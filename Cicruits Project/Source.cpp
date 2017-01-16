#include "Components.h"

int main()
{
	vector<Node> nodes;
	vector<Component*> components;
	do {
		nodes.clear();
		components.clear();
		LoadCircuit(nodes, components);
		cout << endl << "Checking Connections ......" << endl;
		if (CheckForOC(components)) 
			cout << "Connections Error Found , Please reCheck your Connections and try Again";
	} while (CheckForOC(components));
	cout << endl << "Connections Verified , Proceeding";
	PerformNodeAnalysis(nodes);
	cin.get();
	cin.get();
	return 0;
}