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

	//double RTH = GettinTheveninResistance(nodes, 1, 2, "R2");
	
	/*string label;
	cout<<"Choose Desired Element for Vthiev"<<endl;
	cin>>label;
	cout<<endl<<endl;
	cout<<endl<<CalcuateVThevenin(label,components,nodes);*/
	
	Solve(out, nodes, components);
	//cout <<"\n\n\n"<< RTH << "Ohm\n";
	//system("pause");
	cin.get();
	cin.get();
	return 0;
}