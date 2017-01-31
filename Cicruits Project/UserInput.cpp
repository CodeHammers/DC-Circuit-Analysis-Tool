#include "Components.h"

bool CheckForOC(vector<Component*> components,vector<Node> nodes) {
	for (int i = 0; i < components.size(); i++) {
		if (components[i]->Terminal2 == -1) {
			return true;
		}
	}
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].CurrentSource.size() + nodes[i].VoltageSource.size() + nodes[i].Resistors.size() < 2)
			return true;
	}
	return false;
}
bool CheckPolarity(vector<Component*> components) {
	for (int i = 0; i < components.size(); i++) {
		if (components[i]->T1Sign == components[i]->T2Sign && components[i]->Label[0] != 'R') {
			return true;
		}
	}
	return false;
}