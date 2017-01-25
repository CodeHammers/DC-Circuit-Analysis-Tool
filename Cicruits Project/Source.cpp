#include "Components.h"

int main()
{
	vector<Node> nodes;
	vector<Component*> components;
	LoadCircuit(nodes, components);
	PerformNodeAnalysis(nodes);
	cin.get();
	cin.get();
	return 0;
}