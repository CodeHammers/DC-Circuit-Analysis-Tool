#include "Components.h"

int main()
{
	vector<Node> nodes;
	LoadCircuit(nodes);
	PerformNodeAnalysis(nodes);
	cin.get();
	cin.get();
	return 0;
}