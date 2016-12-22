#include "Components.h"


bool checkEssential(Node* node) {
	int totalSize = node->Resistors.size() + node->CurrentSource.size() + node->VoltageSource.size();
	if (totalSize < 2)
		return true;
	return false;

}
double calculateG(Node* node) {
	double TotalG = 0;
	for (std::vector<Component>::iterator it = node->Resistors.begin(); it != node->Resistors.end(); ++it) 
		TotalG += 1/it->Magnitude;
	return TotalG;
}