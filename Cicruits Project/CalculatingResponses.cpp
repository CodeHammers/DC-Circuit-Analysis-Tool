#include "Responses.h"
bool SearchElement(string element, vector<Node>nodes, int &x, int &y)
{
	if (element[0] == 'V') {
		for (int i = 0; i < nodes.size(); i++) {
			for (int k = 0; k < nodes[i].VoltageSource.size(); k++) {
				if (nodes[i].VoltageSource[k]->Label == element) {
					x = i, y = k;   //x= node number, y=index of the element in the vector
					return true;
				}
			}
		}
	}
	else if (element[0] == 'R') {
		for (int i = 0; i < nodes.size(); i++) {
			for (int k = 0; k < nodes[i].Resistors.size(); k++) {
				if (nodes[i].Resistors[k]->Label == element) {
					x = i, y = k;   //x= node number, y=index of the element in the vector
					return true;
				}
			}
		}
	}
	else if (element[0] == 'J') {
		for (int i = 0; i < nodes.size(); i++) {
			for (int k = 0; k < nodes[i].CurrentSource.size(); k++) {
				if (nodes[i].CurrentSource[k]->Label == element) {
					x = i, y = k;   //x= node number, y=index of the element in the vector
					return true;
				}
			}
		}
	}
	else
		return false;
}


double CalculateVD(int node1, int node2, vector<Node>nodes)
{
	double VoltageDifference = nodes[node1-1].voltage - nodes[node2 - 1].voltage;
	return VoltageDifference;
}


double ResistorCurrent(string resistor, vector<Node>nodes)
{
	int x, y;
	if (!SearchElement(resistor, nodes, x, y))
		cout << "Error, element wasn't found" << endl;

	double VD = CalculateVD(nodes[x].Resistors[y]->Terminal1+1,
		                    nodes[x].Resistors[y]->Terminal2+1, nodes);

	double Current = abs((double)VD) / (double)nodes[x].Resistors[y]->Magnitude;

	return Current;
}


double CalculatePower(string element, vector<Node>nodes)
{
	int x, y; double Power;

	if (!SearchElement(element, nodes, x, y)) {
		cout << "Error, element wasn't found" << endl;
		return 0;
	}

	if (element[0] == 'V') {
		double Current = VoltageSourceCurrent(element, nodes);
		double Voltage = nodes[x].VoltageSource[y]->Magnitude;

		Power = Voltage*Current;
		return Power;
	}
	else if (element[0] == 'R') {
		int TR1 = nodes[x].Resistors[y]->Terminal1;
		int TR2 = nodes[x].Resistors[y]->Terminal2;

		double R = nodes[x].Resistors[y]->Magnitude;
		double VD = CalculateVD(TR1 + 1, TR2 + 1, nodes);

		Power = pow(VD, 2) / R;
		return Power;
	}
	else if (element[0] == 'J') {
		int TJ1 = nodes[x].CurrentSource[y]->Terminal1;
		int TJS1 = nodes[x].CurrentSource[y]->T1Sign;
		int TJ2 = nodes[x].CurrentSource[y]->Terminal2;
		int TJS2 = nodes[x].CurrentSource[y]->T2Sign;

		double I = nodes[x].CurrentSource[y]->Magnitude;
		
		double VD;
		if(TJS1 == 1)
			VD = CalculateVD(TJ1 + 1, TJ2 + 1, nodes);
		else
			VD = CalculateVD(TJ2 + 1, TJ1 + 1, nodes);

		Power = VD*I;
		return Power;
	}
}


double VoltageSourceCurrent(string element, vector<Node>nodes)
{
	int x, y, TV1, TV2,TVS1,TVS2,TargetNode, TargetNodeS;

	if (!SearchElement(element, nodes, x, y))
		cout << "Error, element wasn't found" << endl;

	TV1 = nodes[x].VoltageSource[y]->Terminal1;
	TVS1 = nodes[x].VoltageSource[y]->T1Sign;
	TV2 = nodes[x].VoltageSource[y]->Terminal2; 
	TVS2 = nodes[x].VoltageSource[y]->T2Sign;

	if (nodes[TV1].Resistors.size() == 1) {
		TargetNode = TV1;
		TargetNodeS = TVS1;
	}
	else if (nodes[TV2].Resistors.size() == 1) {
		TargetNode = TV2;
		TargetNodeS = TVS2;
	}
	else
		cout << "Error in connections" << endl;

	int TR1 = nodes[TargetNode].Resistors[0]->Terminal1;
	int TR2 = nodes[TargetNode].Resistors[0]->Terminal2;

	double VD;
	if (TargetNode == TR1) 
		VD = CalculateVD(TR1 + 1, TR2 + 1, nodes);
	else 
		VD = CalculateVD(TR2 + 1, TR1 + 1, nodes);

	double Current = (double)VD/(double)nodes[TargetNode].Resistors[0]->Magnitude;

	return Current * TargetNodeS;
}


void Solve(ofstream &out,vector<Node> &nodes,vector<Component*> components)
{
	double current, voltage, power,Rmax; int T1, T2;
	string element, state, response,dueTo, more = "Y";

	while (more != "N") {
		cout << "Enter the type of response you are interested in" << endl;
		cin >> response;

		if (response[0] == 'I') {
			cin >> state;
			if (state == "gen") {
				cin >> element;
				if (element[0] == 'R')
					current = ResistorCurrent(element, nodes);
				else if (element[0] == 'V')
					current = VoltageSourceCurrent(element, nodes);
			}
			else if (state == "spe") {
				cin >> element >> dueTo;
				//to be done later, superposition.
			}
			cout << "The current passing in " << element << " = " << current << endl;
			out << "The current passing in " << element << " = " << current << endl;
		}

		else if (response[0] == 'V') {
			cin >> state;
			if (state == "gen") {
				cin >> T1 >> T2;
				voltage = CalculateVD(T1, T2, nodes);
			}
			else if (state == "spe") {
				cin >> T1 >> T2 >> dueTo;
				//to be done later, superposition.
			}
			cout << "The voltage between node " << T1 << "and node " << T2 << " = " << voltage << endl;
			out << "The voltage between node " << T1 << "and node " << T2 << " = " << voltage << endl;
		}

		else if (response[0] == 'P') {
			cin >> element;
			power = CalculatePower(element, nodes);
			cout << "The power in " << element << " = " << power << endl;
			out << "The power in " << element << " = " << power << endl;
		}

		else if (response == "Rmax") {
			cin >> state >> element;
			power = CalculateMaxPower(element, nodes, components,Rmax);
			cout << "The value of " << element << " so that it draws max power = " << Rmax << endl;
			out << "The value of " << element << " so that it draws max power = " << Rmax << endl;
			cout << "The max power in " << element << " = " << power << endl;
			out << "The max power in " << element << " = " << power << endl;
		}
		cout << "Want to get more responses ? Y or N" << endl;
		cin >> more;
	}
	cout << "An output file has been produced with all the needed reponses" << endl;
	cout << "END" << endl;
}


double CalcuateVThevenin(string label, vector<Component*> components, vector<Node> nodes) {
	Node* node1, *node2;
	Component* comp = NULL;
	int pupput, pos1, pos2;
	for (int i = 0; i < components.size(); i++) {
		if (components[i]->Label == label) {
			comp = components[i];
		}
	}
	node1 = &nodes[comp->Terminal1];
	node2 = &nodes[comp->Terminal2];
	switch (label[0]) {
	case 'V':
		for (int i = 0; i < node1->VoltageSource.size(); i++) {
			if (node1->VoltageSource[i]->Label == label) {
				pos1 = i;
			}
		}
		for (int i = 0; i < node2->VoltageSource.size(); i++) {
			if (node2->VoltageSource[i]->Label == label) {
				pos2 = i;
			}
		}
		node1->VoltageSource.erase(node1->VoltageSource.begin() + pos1);
		node2->VoltageSource.erase(node2->VoltageSource.begin() + pos1);
		break;
	case 'J':
		for (int i = 0; i < node1->CurrentSource.size(); i++) {
			if (node1->CurrentSource[i]->Label == label) {
				pos1 = i;
			}
		}
		for (int i = 0; i < node2->CurrentSource.size(); i++) {
			if (node2->CurrentSource[i]->Label == label) {
				pos2 = i;
			}
		}
		node1->CurrentSource.erase(node1->CurrentSource.begin() + pos1);
		node2->CurrentSource.erase(node2->CurrentSource.begin() + pos1);
		break;
	case 'R':
		for (int i = 0; i < node1->Resistors.size(); i++) {
			if (node1->Resistors[i]->Label == label) {
				pos1 = i;
			}
		}
		for (int i = 0; i < node2->Resistors.size(); i++) {
			if (node2->Resistors[i]->Label == label) {
				pos2 = i;
			}
		}
		node1->Resistors.erase(node1->Resistors.begin() + pos1);
		node2->Resistors.erase(node2->Resistors.begin() + pos2);
		break;
	default:
		throw new exception();
	}
	PerformNodeAnalysis(nodes);
	return CalculateVD(node1->Number + 1, node2->Number + 1, nodes);
}


double CalculateMaxPower(string element,vector<Node>nodes,vector<Component*> components,double &Rth)
{
	int x, y;
	SearchElement(element, nodes, x, y);

	int T1 = nodes[x].Resistors[y]->Terminal1;
	int T2 = nodes[x].Resistors[y]->Terminal2;
	cout << endl << T1 << " " << T2 << endl;

	Rth = GettinTheveninResistance(nodes, T1, T2, element);
	double Vth = CalcuateVThevenin(element, components, nodes);

	double MaxPower = (Vth*Vth) / (4 * Rth);

	return MaxPower;
}


bool PowerBalance(vector<Component*> components, vector<Node>nodes, 
	              double &DissPower, double &SupPower)
{
	double temp;  DissPower = 0, SupPower = 0;

	for (int i = 0; i < components.size(); i++) {
		temp = CalculatePower(components[i]->Label, nodes);

		if (components[i]->Label[0] == 'R') 
			DissPower += temp;
		

		else if (components[i]->Label[0] == 'J' || components[i]->Label[0] == 'V') {
			if (temp < 0)
				DissPower += abs(temp);
			else
				SupPower += temp;
		}
	}

	if (abs(DissPower - SupPower) < 10e-3)
		return true;
	else
		return false;
}