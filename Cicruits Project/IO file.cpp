#include "IO.h"

bool FirstAppear(string name, vector<Component>Aux)
{
	int Size = Aux.size();
	for (int i = 0;i < Size;++i)
		if (Aux[i].Label == name)
			return false;
	return true;
}
void LoadFile(vector<Node> &Circuit,vector<Component>&Resistors,vector<Component>&VoltageSources, vector<Component>&CurrentSources)
{
	string TempName;
	float value;
	Component Helping;
	Node Help;
	bool exit = false;
	int Terminate;
	int count = 1;
	while (!exit) {
		Help.Number = count;

		Help.CurrentSource.clear();
		Help.Resistors.clear();
		Help.VoltageSource.clear();

		cout << "Please Enter All Elements Connected to The Node # " << count << "\n";
		cin >> Terminate;
		if (Terminate != 1) break;
		while (1) {
			cin >> TempName;
			if (TempName == "-1")
				break;

			cin >> value;

			Helping.Label = TempName;
			Helping.Magnitude = value;
			Helping.Terminal1 = count;
			Helping.Terminal2 = -1;

			if (TempName[0] == 'R')
				Help.Resistors.push_back(Helping);
			
			else if (TempName[0] == 'E')
				Help.VoltageSource.push_back(Helping);
			
			else
				Help.CurrentSource.push_back(Helping);
			
		}
		
		Circuit.push_back(Help);
		count++;
	}
	UpgradingTerminals(Circuit);
	UpgradingVectors(Circuit,Resistors, VoltageSources, CurrentSources);
}
int WhichNodeIsConnected(vector<Node>Circuit, int FirstNode, string name)
{
	int SizeOFNodes = Circuit.size();
	for (int i = 0;i < SizeOFNodes;++i)
	{
		if (FirstNode == i + 1) continue;

		if (name[0] == 'R')
		{
			int SizeOfResistors = Circuit[i].Resistors.size();
			for (int j = 0;j < SizeOfResistors;++j)
				if (Circuit[i].Resistors[j].Label == name)
					return i + 1;
		}
		else if (name[0] == 'E')
		{
			int SizeOfVoltageSources = Circuit[i].VoltageSource.size();
			for (int j = 0;j < SizeOfVoltageSources;++j)
				if (Circuit[i].VoltageSource[j].Label == name)
					return i + 1;
		}
		else
		{
			int CurrentSources = Circuit[i].CurrentSource.size();
			for (int j = 0;j < CurrentSources;++j)
				if (Circuit[i].CurrentSource[j].Label == name)
					return i + 1;
		}
	}

	return -1;
}
void UpgradingTerminals(vector<Node>&Circuit)
{
	int SizeOFNodes = Circuit.size();
	for (int i = 0;i < SizeOFNodes;++i)
	{
		int FirstNode = i + 1;

		/* Upgrading All Voltage Sources Connected To The Node # i+1 */

		int SizeOFVotageSources = Circuit[i].VoltageSource.size();
		for (int j = 0;j < SizeOFVotageSources;++j)		
			Circuit[i].VoltageSource[j].Terminal2 = WhichNodeIsConnected(Circuit, FirstNode, Circuit[i].VoltageSource[j].Label);

		/* Upgrading All Voltage Sources Connected To The Node # i+1 */

		int SizeOFCurrentSources = Circuit[i].CurrentSource.size();
		for (int j = 0;j < SizeOFCurrentSources;++j)
			Circuit[i].CurrentSource[j].Terminal2 = WhichNodeIsConnected(Circuit, FirstNode, Circuit[i].CurrentSource[j].Label);

		/* Upgrading All Voltage Sources Connected To The Node # i+1 */

		int SizeOFResistors = Circuit[i].Resistors.size();
		for (int j = 0;j < SizeOFResistors;++j)
			Circuit[i].Resistors[j].Terminal2 = WhichNodeIsConnected(Circuit, FirstNode, Circuit[i].Resistors[j].Label);

	}
}
void UpgradingVectors(vector<Node>Circuit, vector<Component>&Resistors, vector<Component>&VoltageSources, vector<Component>&CurrentSources)
{
	int SizeOFNodes = Circuit.size();

	for (int i = 0;i < SizeOFNodes;++i)
	{
		int SizeOFBatteries = Circuit[i].VoltageSource.size();
		for (int j = 0;j < SizeOFBatteries;++j)
			if (FirstAppear(Circuit[i].VoltageSource[j].Label, VoltageSources))
				VoltageSources.push_back(Circuit[i].VoltageSource[j]);

		int SizeOFResistors = Circuit[i].Resistors.size();
		for (int j = 0;j < SizeOFResistors;++j)
			if (FirstAppear(Circuit[i].Resistors[j].Label, Resistors))
				Resistors.push_back(Circuit[i].Resistors[j]);

		int SizeOFCurrentSources = Circuit[i].CurrentSource.size();
		for (int j = 0;j < SizeOFCurrentSources;++j)
			if (FirstAppear(Circuit[i].CurrentSource[j].Label, CurrentSources))
				CurrentSources.push_back(Circuit[i].CurrentSource[j]);
	}
}
int GetTheIndex(string name	, vector<Component> Objects)
{
	int Size = Objects.size();
	for (int i = 0;i < Size;++i)
		if (Objects[i].Label == name)
			return i;
	return -1;
}
void ConnectElementsToNode(vector<NewNode>&Circuit, vector<Component> &VoltageSources, vector<Component> &CurrentSources,
							vector<Component>&Resistors,int i,vector<Component>Aux,int SizeOfNodes)
{
	// (i) is The index of the node
	int size = Circuit.size();
	if (size - 1 < i)
		Circuit.resize(Circuit.size() + 1), Circuit[i].Number = i + 1;


	int Size= Aux.size();
	if (Aux[0].Label[0] == 'R')
		for (int j = 0;j < Size;++j)
			Circuit[i].Resistors.push_back(&Resistors[GetTheIndex(Aux[j].Label, Resistors)]);


	else if (Aux[0].Label[0] == 'E')
		for (int j = 0;j < Size;++j)
			Circuit[i].VoltageSource.push_back(&VoltageSources[GetTheIndex(Aux[j].Label, VoltageSources)]);

	else
		for (int j = 0;j < Size;++j)
			Circuit[i].CurrentSource.push_back(&CurrentSources[GetTheIndex(Aux[j].Label, CurrentSources)]);
}
vector <NewNode>TheCircuitIs(vector<Node>Circuit, vector<Component> &VoltageSources, vector<Component> &CurrentSources, vector<Component>&Resistors)
{
	/*This Is The Real Circuit With All Objects with Our Restrictions*/
	vector<NewNode>New;
	
	int SizeOfNodes = Circuit.size();
	for (int i = 0;i < SizeOfNodes;++i)
	{
		int SizeOFBatteries = Circuit[i].VoltageSource.size();
		vector<Component> Aux;
		
		for (int j = 0;j < SizeOFBatteries;++j)
			Aux.push_back(Circuit[i].VoltageSource[j]);

		if (Aux.size() != 0)
			ConnectElementsToNode(New, VoltageSources, CurrentSources, Resistors, i, Aux, SizeOfNodes), Aux.clear();

		int SizeOFCurrentSources = Circuit[i].CurrentSource.size();
		for (int j = 0;j < SizeOFCurrentSources;++j)
			Aux.push_back(Circuit[i].CurrentSource[j]);

		if (Aux.size() != 0)
		ConnectElementsToNode(New, VoltageSources, CurrentSources, Resistors, i, Aux, SizeOfNodes), Aux.clear();


		int SizeOFResistors = Circuit[i].Resistors.size();
		for (int j = 0;j < SizeOFResistors;++j)
			Aux.push_back(Circuit[i].Resistors[j]);

		if (Aux.size() != 0)
			ConnectElementsToNode(New, VoltageSources, CurrentSources, Resistors, i, Aux, SizeOfNodes), Aux.clear();
	}
	return New;
}