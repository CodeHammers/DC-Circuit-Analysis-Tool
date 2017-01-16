#include "Components.h"

bool CheckForOC(vector<Component*> components) {
	for (int i = 0; i < components.size(); i++) {
		if (components[i]->Terminal2 == -1) {
			return true;
		}
	}
	return false;
}