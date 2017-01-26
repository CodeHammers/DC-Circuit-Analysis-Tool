#include "Components.h"
#include "Responses.h"

int main()
{
	ofstream out; ifstream input;
	input.open("input.txt", ios::in);
	out.open("output.txt", ios::out);
	vector<Node> nodes;
	vector<Component*> components;
	LoadCircuit(nodes, components);
	PerformNodeAnalysis(nodes);
	/*
	string label;
	cout<<"Choose Desired Element for Vthiev"<<endl;
	cin>>label;
	cout<<endl<<endl;
	cout<<CalcuateVThevenin(label,components,nodes);
	*/
	Solve(out, nodes);
	cin.get();
	cin.get();
	return 0;
}