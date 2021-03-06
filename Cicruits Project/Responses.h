#pragma once
#include "Components.h"

bool SearchElement(string element, vector<Node>nodes, int &x, int &y);

double CalculateVD(int node1, int node2, vector<Node>nodes);

double ResistorCurrent(string resistor, vector<Node>nodes);

double CalculatePower(string element, vector<Node>nodes);

double VoltageSourceCurrent(string element, vector<Node>nodes);

void Solve(ofstream &out, vector<Node> &nodes,vector<Component*> components);

double CalcuateVThevenin(string label, vector<Component*> components, vector<Node> nodes);

double CalculateMaxPower(string element, vector<Node>nodes, vector<Component*> components, double &Rth);

bool PowerBalance(vector<Component*> components, vector<Node>nodes,
	double &DissPower, double &SupPower);