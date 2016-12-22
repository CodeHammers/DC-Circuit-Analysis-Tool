#include "Components.h"


bool CheckEssential(Node* node) {
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
double CalculateMutualG(Node* node1,Node* node2) {
	double TotalG = 0;
	for (std::vector<Component>::iterator it = node1->Resistors.begin(); it != node1->Resistors.end(); ++it)
		if( (it->Terminal1==node1->Number&&it->Terminal2 == node2->Number) || (it->Terminal2 == node1->Number&&it->Terminal1 == node2->Number) )
			TotalG += 1 / it->Magnitude;
	return TotalG;
}
