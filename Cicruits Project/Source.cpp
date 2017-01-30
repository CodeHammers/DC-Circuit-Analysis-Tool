#include "Components.h"
#include "Responses.h"

int main()
{
	ofstream out; ifstream input;
	input.open("input.txt", ios::in);
	out.open("output.txt", ios::out);
	vector<Node> nodes; double Diss = 0, Sup = 0;
	vector<Component*> components;

	LoadCircuit(nodes, components);
	if (CheckForOC(components, nodes)) {
		cout << endl << endl << "Error, Bad Connections";
		cout <<endl<< "please try again " << " . thank you for your patience ";
		cin.get();
		cin.get();
		return 0;
	}
	PerformNodeAnalysis(nodes);
	cout << endl << endl;
	bool flag = PowerBalance(components,nodes, Diss, Sup);
	cout << "Dissipated Power = " << Diss << endl;
	cout << "Supplied Power = " << Sup << endl;
	if (flag)
		cout << "Balanced" << endl;
	else
		cout << "Not balanced" << endl;	
	Solve(out, nodes, components);
	cin.get();
	cin.get();
	return 0;
}