#include "Components.h"


bool CheckEssential(Node* node) {
	//Please note : voltageSources should be zero  in node analysis , otherwise you should know what you are doing
	int totalSize = node->Resistors.size() + node->CurrentSource.size() + node->VoltageSource.size();
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
//calculates I for current Sources  between two nodes
double CalculateMutualCurrent(Node* node1, Node* node2) {
	double TotalI = 0;
	for (std::vector<Component>::iterator it = node1->CurrentSource.begin(); it != node1->CurrentSource.end(); ++it)
		if ((it->Terminal1 == node1->Number&&it->Terminal2 == node2->Number) || (it->Terminal2 == node1->Number&&it->Terminal1 == node2->Number))
			TotalI += 1 / it->Magnitude;
	return TotalI;
}