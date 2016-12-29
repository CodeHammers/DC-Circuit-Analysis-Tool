#include "Components.h"


bool CheckEssential(Node* node) {
	//Please note : voltageSources should be zero  in node analysis , otherwise you should know what you are doing
	int totalSize = node->Resistors.size() + node->CurrentSource.size();
	if (totalSize < 2)
		return true;
	return false;

}
double CalculateG(Node* node) {
	double TotalG = 0;
	for (std::vector<Component>::iterator it = node->Resistors.begin(); it != node->Resistors.end(); ++it) 
		TotalG += 1/it->Magnitude;
	return TotalG;
}
//calculates G  for resistors  between two nodes
double CalculateMutualG(Node* node1,Node* node2) {
	if (node1 == node2)
		return CalculateG(node1);
	double TotalG = 0;
	for (std::vector<Component>::iterator it = node1->Resistors.begin(); it != node1->Resistors.end(); ++it)
		if( (it->Terminal1==node1->Number&&it->Terminal2 == node2->Number) || (it->Terminal2 == node1->Number&&it->Terminal1 == node2->Number) )
			TotalG += 1 / it->Magnitude;
	return TotalG;
}
double CalculateCurrent(Node* node) {
	double TotalI = 0;
	for (std::vector<Component>::iterator it = node->CurrentSource.begin(); it != node->CurrentSource.end(); ++it)
		TotalI += 1 / it->Magnitude;
	return TotalI;
}
//TODO: Function to distinguish the ref node 
bool isRef(Node* node) {
	return false;
}
void BindVoltageValues(vector<Node> nodes,MatrixXd matrixV) {
	int matIndex = 0;
	for (int i = 0;i<nodes.size();i++) 
		if (CheckEssential(&nodes[i])) {
			nodes[i].voltage = matrixV(matIndex, 0);
			matIndex++;
		}
}
int CountEssentialNodes(vector<Node> nodes) {
	int EssentialsCount = 0;
	for (std::vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		if (CheckEssential(&(*it)))
			EssentialsCount++;
	}
	return EssentialsCount;
}
//returns the vector of nodes with the voltages values
//attached to each node
void PerformNodeAnalysis(vector<Node> nodes) {
	//Should there be a call to convert circuit first 
	int EssetialCount = CountEssentialNodes(nodes);
	MatrixXd matG(EssetialCount, EssetialCount);
	MatrixXd matI(EssetialCount, 1);
	MatrixXd matV(EssetialCount, 1);
	matG= BuildMatrixG(nodes);
	matI = BuildMatrixI(nodes);
	matV = GetMatrixV(matG, matI);
	BindVoltageValues(nodes,matV);
}