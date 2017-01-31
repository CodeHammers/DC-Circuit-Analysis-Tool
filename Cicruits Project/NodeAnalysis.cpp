#include "Components.h"

double GettinTheveninResistance(vector<Node>Circuits, int A, int B, string name)
{

	for (int i = 0; i < Circuits[A].Resistors.size(); ++i)
		if (Circuits[A].Resistors[i]->Label == name)
			Circuits[A].Resistors.erase(Circuits[A].Resistors.begin() + i);

	for (int i = 0; i < Circuits[B].Resistors.size(); ++i)
		if (Circuits[B].Resistors[i]->Label == name)
			Circuits[B].Resistors.erase(Circuits[B].Resistors.begin() + i);
	Circuits[A].isRef = true;
	Circuits[B].isRef = true;

	ConvertCircuit(Circuits);
	Circuits[A].isRef = false;
	Circuits[B].isRef = false;

	// R Load Deleted 
	Node NewNode;
	NewNode.Number = Circuits.size();
	NewNode.isRef = NewNode.deprecated = NewNode.voltageSet = false;
	Component *VTemp = new Component();
	VTemp->Label = "ETemp";  VTemp->Magnitude = 1;
	VTemp->Terminal1 = NewNode.Number;	VTemp->Terminal2 = B;
	VTemp->T1Sign = 1; VTemp->T2Sign = -1;

	Component *RTemp = new Component();
	RTemp->Label = "RTemp"; RTemp->Magnitude = 1;
	RTemp->T1Sign = RTemp->T2Sign = 1;
	RTemp->Terminal1 = A; RTemp->Terminal2 = NewNode.Number;

	Circuits.push_back(NewNode);
	Circuits[A].Resistors.push_back(RTemp);
	Circuits[NewNode.Number].VoltageSource.push_back(VTemp);
	Circuits[NewNode.Number].Resistors.push_back(RTemp);
	Circuits[B].VoltageSource.push_back(VTemp);

	PerformSuperPosition(Circuits, "ETemp");

	//Now We Added The R 1 Ohm And E 1 Volt //

	return (1 / abs((Circuits[A].voltage - Circuits[NewNode.Number].voltage))) - 1;
}

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
	if (node1->Number == node2->Number)
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
double CalculateCurrent(Node* node,string label) {
	double TotalI = 0;
	for (int i = 0; i < node->CurrentSource.size(); i++) {
		Component* CS = node->CurrentSource[i];
		if (CS->Label == label) {
			if (CS->Terminal1 == node->Number)
				TotalI += CS->Magnitude * CS->T1Sign;
			else
				TotalI += CS->Magnitude * CS->T2Sign;
		}
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
			validToConvert = validToConvert && RessBelongsToVS( nodes[i].Resistors[0],nodes[i],nodes);
			validToConvert = validToConvert && !nodes[i].deprecated;
			if (validToConvert) {
				Node* N = &nodes[i];
				ConvertVStoCS(N, nodes);
			}
		}
	}
}
bool RessBelongsToVS(Component* Ress,Node node,vector<Node> nodes) {
	int FreeTerminalForRes = Ress->Terminal1 == node.Number ? Ress->Terminal2 : Ress->Terminal1;
	Node* FreeNode = &nodes[FreeTerminalForRes];
	if (CheckEssential(FreeNode) || FreeNode->deprecated || FreeNode->VoltageSource.empty())
		return true;
	Component * Batt = FreeNode->VoltageSource[0];
	int FreeTerminalForBatt = Batt->Terminal1 == node.Number ? Batt->Terminal2 : Batt->Terminal1;
	if (nodes[FreeTerminalForBatt].deprecated)
		return false;
	if (CheckEssential(&nodes[FreeTerminalForBatt]))
		return false;
	if (nodes[FreeTerminalForBatt].Resistors.empty())
		return false;
	return true;
}
void ConvertVStoCS(Node* &node, vector<Node> &nodes) {
	node->deprecated = true;
	Component* CurrentSource = new Component();
	Component* Battery = node->VoltageSource[0];
	Component* Resistor = node->Resistors[0];
	CurrentSource->Label = "J"+Battery->Label;
	CurrentSource->Magnitude = Battery->Magnitude / Resistor->Magnitude;
	int BoundryFromRessSide = GetBoundryNode(Resistor, node);
	int BoundryFromBattSide = GetBoundryNode(Battery, node);
	if (nodes[BoundryFromBattSide].deprecated) {
		node->deprecated = false;
		return;
	}
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
void DeConvertCircuit(vector<Node> &nodes,string operation,string label) {
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].deprecated == true && !nodes[i].isRef) {
			Node* N = &nodes[i];
			RollBackChangesToNode(N, nodes,operation,label);
		}
	}
}
void RollBackChangesToNode(Node* &node, vector<Node> &nodes,string operation,string label) {
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
	if (operation == "nodeanaylsis" || label == Battery->Label) {
		if (Battery->Terminal1 == BoundryFromBattSide)
			node->voltage = nodes[BoundryFromBattSide].voltage - Battery->T1Sign * Battery->Magnitude;
		else
			node->voltage = nodes[BoundryFromBattSide].voltage - Battery->T2Sign * Battery->Magnitude;
	}
	else {
		node->voltage = nodes[BoundryFromBattSide].voltage;
	}

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
	DeConvertCircuit(nodes,"nodeanaylsis","");
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].deprecated)
			nodes[i].deprecated = false;
		if (nodes[i].isRef)
			nodes[i].isRef = false;
	}
}
void PerformSuperPosition(vector<Node> &nodes,string label) {
	ConvertCircuit(nodes);
	int ActualSize = GetActualSize(nodes);
	MatrixXd matG(ActualSize - 1, ActualSize - 1);
	MatrixXd matI(ActualSize - 1, 1);
	MatrixXd matV(ActualSize - 1, 1);
	SetRefNode(nodes);
	matG = BuildMatrixG(nodes);
	matI = BuildMatrixI(nodes,label);
	matV = GetMatrixV(matG, matI);
	BindVoltageValues(nodes, matV);
	DeConvertCircuit(nodes,"superposition",label);
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].deprecated)
			nodes[i].deprecated = false;
		if (nodes[i].isRef)
			nodes[i].isRef = false;
	}
}