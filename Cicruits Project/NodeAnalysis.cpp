#include "Components.h"


bool CheckEssential(Node* node) {
	//Please note : voltageSources should be zero  in node analysis , otherwise you should know what you are doing
	int totalSize = node->Resistors.size() + node->CurrentSource.size() + node->VoltageSource.size();
	if (totalSize > 2)
		return true;
	return false;

}
double CalculateG(Node* node) {
	double TotalG = 0;
	for (int i = 0; i < node->Resistors.size(); i++)
		TotalG += 1.0 / node->Resistors[i]->Magnitude;
	return TotalG;
}
//calculates G  for resistors  between two nodes
double CalculateMutualG(Node* node1, Node* node2) {
	if (node1 == node2)
		return CalculateG(node1);
	double TotalG = 0;
	for (int i = 0; i < node1->Resistors.size(); i++) {
		Component* ress = node1->Resistors[i];
		if ((ress->Terminal1 == node1->Number && ress->Terminal2 == node2->Number) ||
			(ress->Terminal2 == node1->Number && ress->Terminal1 == node2->Number))
			TotalG += 1.0 / ress->Magnitude;
	}
	return TotalG;
}
double CalculateCurrent(Node* node) {
	double TotalI = 0;
	for (int i = 0; i < node->CurrentSource.size(); i++) {
		Component* CS = node->CurrentSource[i];
		if (CS->Terminal1 == node->Number)
			TotalI += CS->Magnitude * CS->T1Sign;
		else
			TotalI += CS->Magnitude * CS->T2Sign;
	}
	return TotalI;
}
//TODO: Function to distinguish the ref node 
bool isRef(Node* node) {
	return false;
}
void BindVoltageValues(vector<Node> &nodes, MatrixXd matrixV) {
	int matIndex = 0;
	for (int i = 0; i < nodes.size(); i++)
		if (!nodes[i].deprecated) {
			nodes[i].voltage = matrixV(matIndex, 0);
			nodes[i].voltageSet = true;
			matIndex++;
		}
		else
			nodes[i].voltageSet = false;
}

void ConvertCircuit(vector<Node> &nodes) {
	for (int i = 0; i < nodes.size(); i++) {
		if (!CheckEssential(&nodes[i]) && !nodes[i].isRef) {
			bool validToConvert = !(nodes[i].Resistors.empty() || nodes[i].VoltageSource.empty());
			if (validToConvert) {
				ConvertVStoCS(&nodes[i], nodes);
			}
		}
	}
}
void ConvertVStoCS(Node* node, vector<Node> &nodes) {
	node->deprecated = true;
	Component* CurrentSource = new Component();
	Component* Battery = node->VoltageSource[0];
	Component* Resistor = node->Resistors[0];
	CurrentSource->Label = "SayedTito+MoamenDarwish";
	CurrentSource->Magnitude = Battery->Magnitude / Resistor->Magnitude;
	int BoundryFromRessSide = GetBoundryNode(Resistor, node);
	int BoundryFromBattSide = GetBoundryNode(Battery, node);
	if (Resistor->Terminal1 == node->Number)
		Resistor->Terminal1 = BoundryFromBattSide;
	else
		Resistor->Terminal2 = BoundryFromBattSide;
	nodes[BoundryFromBattSide].Resistors.push_back(Resistor);
	CurrentSource->T1Sign = Battery->T1Sign;
	CurrentSource->T2Sign = Battery->T2Sign;
	if (Battery->Terminal1 == node->Number) {
		CurrentSource->Terminal1 = BoundryFromRessSide;
		CurrentSource->Terminal2 = BoundryFromBattSide;
	}
	else {
		CurrentSource->Terminal2 = BoundryFromRessSide;
		CurrentSource->Terminal1 = BoundryFromBattSide;
	}
	nodes[BoundryFromBattSide].CurrentSource.push_back(CurrentSource);
	nodes[BoundryFromRessSide].CurrentSource.push_back(CurrentSource);
}
int GetBoundryNode(Component* cmp, Node * node) {
	if (cmp->Terminal1 == node->Number)
		return cmp->Terminal2;
	return cmp->Terminal1;
}
void DeConvertCircuit(vector<Node> nodes) {
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].deprecated == true && !nodes[i].isRef) {
			RollBackChangesToNode(&nodes[i], nodes);
		}
	}
}
void RollBackChangesToNode(Node* node, vector<Node> &nodes) {
	Component* Battery = node->VoltageSource[0];
	Component* Resistor = node->Resistors[0];
	int BoundryFromBattSide = GetBoundryNode(Battery, node);
	int BoundryFromRessSide = GetBoundryNode(Resistor, &nodes[BoundryFromBattSide]);
	if (Resistor->Terminal1 == BoundryFromBattSide)
		Resistor->Terminal1 = node->Number;
	else
		Resistor->Terminal2 = node->Number;
	nodes[BoundryFromBattSide].Resistors.pop_back();
	nodes[BoundryFromBattSide].CurrentSource.pop_back();
	nodes[BoundryFromRessSide].CurrentSource.pop_back();
	node->voltageSet = true;
	if(Battery->Terminal1 == BoundryFromBattSide)
		node->voltage = nodes[BoundryFromBattSide].voltage - Battery->T1Sign * Battery->Magnitude;
	else
		node->voltage = nodes[BoundryFromBattSide].voltage - Battery->T2Sign * Battery->Magnitude;

}
int GetActualSize(vector<Node> nodes) {
	int actualSize = nodes.size();
	for (int i = 0; i < nodes.size(); i++)
		if (nodes[i].deprecated)
			actualSize--;
	return actualSize;

}
void SetRefNode(vector<Node> &nodes) {
	for (int i = 0; i < nodes.size(); i++) {
		if (!nodes[i].deprecated) {
			nodes[i].deprecated = true;
			nodes[i].isRef = true;
			nodes[i].voltage = 0;
			nodes[i].voltageSet = true;
			return;
		}
	}
}
////returns the vector of nodes with the voltages values
//attached to each node
void PerformNodeAnalysis(vector<Node> &nodes) {
	ConvertCircuit(nodes);
	int ActualSize = GetActualSize(nodes);
	MatrixXd matG(ActualSize - 1, ActualSize - 1);
	MatrixXd matI(ActualSize - 1, 1);
	MatrixXd matV(ActualSize - 1, 1);
	SetRefNode(nodes);
	matG = BuildMatrixG(nodes);
	matI = BuildMatrixI(nodes);
	matV = GetMatrixV(matG, matI);
	BindVoltageValues(nodes, matV);
	DeConvertCircuit(nodes);
}