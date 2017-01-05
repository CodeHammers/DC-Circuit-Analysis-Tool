#pragma
#include "IO.h"
void LoadFile(vector<NewNode>&Circuit, vector<Component>&Resistors, vector<Component>&VoltageSources, vector<Component>&CurrentSources)
{
	string TempName; float value; int Terminate; Component Helping;
	bool exit = false;
	int count = 1;
	while (!exit) {
		NewNode Help;
		cout << "Please Enter All Elements Connected to The Node # " << count << "\n";
		cin >> Terminate;
		if (Terminate != 1) break;
		while (1) {

			bool firstAppear = true;
			cin >> TempName;
			if (TempName == "-1") break;
			cin >> value;
			Helping.Label = TempName;

			if (FirstAppear(TempName, Resistors, VoltageSources, CurrentSources)) {
				Helping.Magnitude = value; Helping.Terminal1 = count;
				Helping.Terminal2 = -1; 	firstAppear = true;
			}
			else
				UpgradeTerminal(TempName, Resistors, VoltageSources, CurrentSources, count), firstAppear = false;

			if (TempName[0] == 'R')
			{
				if (firstAppear)
					Resistors.push_back(Helping);
				Help.Resistors.push_back(&Resistors[GetTheIndex(TempName, Resistors)]);
			}
			else if (TempName[0] == 'E')
			{
				if (firstAppear)
					VoltageSources.push_back(Helping);
				Help.VoltageSource.push_back(&VoltageSources[GetTheIndex(Helping.Label, VoltageSources)]);
			}
			else
			{
				if (firstAppear)
					CurrentSources.push_back(Helping);
				Help.CurrentSource.push_back(&CurrentSources[GetTheIndex(Helping.Label, CurrentSources)]);
			}
		}
		Help.Number = count;
		Circuit.push_back(Help);
		count++;
	}
}
bool FirstAppear(string name, vector<Component>Resistors, vector<Component>VoltageSources, vector<Component>CurrentSources)
{

	if (name[0] == 'R')
	{
		int Size = Resistors.size();
		for (int i = 0;i < Size;++i)
			if (Resistors[i].Label == name)
				return false;
	}
	else if (name[0] == 'E')
	{
		int Size = VoltageSources.size();
		for (int i = 0;i < Size;++i)
			if (VoltageSources[i].Label == name)
				return false;
	}
	else
	{
		int Size = CurrentSources.size();
		for (int i = 0;i < Size;++i)
			if (CurrentSources[i].Label == name)
				return false;
	}
	return true;
}
int GetTheIndex(string name, vector<Component> Aux)
{
	int Size = Aux.size();
	for (int i = 0;i < Size;++i)
		if (Aux[i].Label == name)
			return i;
	return -1;
}
void UpgradeTerminal(string Name, vector<Component>&Resistors, vector<Component>&VoltageSources, vector<Component>&CurrentSources, int NumOfNode)
{
	if (Name[0] == 'R')
	{
		int Size = Resistors.size();
		for (int i = 0;i < Size;++i)
			if (Resistors[i].Label == Name) {
				Resistors[i].Terminal2 = NumOfNode;
				return;
			}
	}
	else if (Name[0] == 'E')
	{
		int Size = VoltageSources.size();
		for (int i = 0;i < Size;++i)
			if (VoltageSources[i].Label == Name) {
				VoltageSources[i].Terminal2 = NumOfNode;
				return;
			}
	}
	else {
		int Size = CurrentSources.size();
		for (int i = 0;i < Size;++i)
			if (CurrentSources[i].Label == Name) {
				CurrentSources[i].Terminal2 = NumOfNode;
				return;
			}
	}
}
