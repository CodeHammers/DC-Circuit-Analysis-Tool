#include <iostream>
#include "Components.h"
//#include <Eigen/Dense>
//using Eigen::MatrixXd;
int main()
{
	vector<Node> AuxCircuit;
	vector<Component>VoltageSources;
	vector<Component>CurrentSources;
	vector<Component>Resistors;
	LoadFile(AuxCircuit,Resistors,VoltageSources,CurrentSources);

	vector<NewNode>Circuit;

	Circuit = TheCircuitIs(AuxCircuit,VoltageSources,CurrentSources,Resistors);

	int Size = Circuit.size();
	for (int i = 0;i < Size;++i)
	{
		cout << "\n\nFor Node # " << i + 1 << "\n\n";

		int Size1 = Circuit[i].VoltageSource.size();
		for (int j = 0;j < Size1;++j)
			cout << Circuit[i].VoltageSource[j]->Label << "\t" << Circuit[i].VoltageSource[j]->Magnitude << "\t"
			<< Circuit[i].VoltageSource[j]->Terminal1 << "\t" << Circuit[i].VoltageSource[j]->Terminal2 << "\n";

		Size1 = Circuit[i].CurrentSource.size();
		for (int j = 0;j < Size1;++j)
			cout << Circuit[i].CurrentSource[j]->Label << "\t" << Circuit[i].CurrentSource[j]->Magnitude << "\t"
			<< Circuit[i].CurrentSource[j]->Terminal1 << "\t" << Circuit[i].CurrentSource[j]->Terminal2 << "\n";

		Size1 = Circuit[i].Resistors.size();
		for (int j = 0;j < Size1;++j)
			cout << Circuit[i].Resistors[j]->Label << "\t" << Circuit[i].Resistors[j]->Magnitude << "\t"
			<< Circuit[i].Resistors[j]->Terminal1 << "\t" << Circuit[i].Resistors[j]->Terminal2 << "\n";
	}


/*	MatrixXd m(2, 2);
	m(0, 0) = 3;
	m(1, 0) = 2.5;
	m(0, 1) = -1;
	m(1, 1) = m(1, 0) + m(0, 1);
	std::cout << m << std::endl;
	std::cin.get();

*/
	system("pause");
	return 0;
}