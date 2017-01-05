#include <iostream>
#include "Components.h"
//#include <Eigen/Dense>
//using Eigen::MatrixXd;
int main()
{
	vector<Component>VoltageSources(50); vector<Component>CurrentSources(50);
	vector<Component>Resistors(50);      vector<NewNode>Circuit;

	LoadFile(Circuit, Resistors, VoltageSources, CurrentSources);
	int Size1 = Resistors.size(); int Size2 = VoltageSources.size(); int Size3 = CurrentSources.size();

	cout << "\n\n\nThat is All Components\n\n";
	cout << "Name\tMagnitude\n";
	for (int j = 0;j < Size1;++j)
		if (Resistors[j].Magnitude != 0)
			cout << Resistors[j].Label << "\t" << Resistors[j].Magnitude << "\n";
	for (int j = 0;j < Size2;++j)
		if (VoltageSources[j].Magnitude != 0)
			cout << VoltageSources[j].Label << "\t" << VoltageSources[j].Magnitude << "\n";
	for (int j = 0;j < Size3;++j)
		if (CurrentSources[j].Magnitude != 0)
			cout << CurrentSources[j].Label << "\t" << CurrentSources[j].Magnitude << "\n";
	int Size = Circuit.size();
	for (int i = 0;i < Size;++i) {
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
	m(1, 1) = 5;
	m = -m;
	m.diagonal() = -m.diagonal();
	std::cout << m << std::endl;
	std::cin.get();


	cin.get();
	*/
	system("pause");
	return 0;
}