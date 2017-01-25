#include "Components.h"

int main()
{
	ofstream out;
	out.open("output.txt", ios::out);
	vector<Node> nodes;
	vector<Component*> components;
	LoadCircuit(nodes, components);
	PerformNodeAnalysis(nodes);
	cin.get();
	cin.get();
	return 0;
}