#pragma once
#include "Libraries.h"

struct Component
{
	float Magnitude;
	string Label;
	int Terminal1;
	int Terminal2;
};

struct Node
{
	int Number;
	double voltage;
	bool deprecated;
	bool voltageSet;
	vector<Component>Resistors;
	vector<Component>CurrentSource;
	vector<Component>VoltageSource;
};

struct NewNode
{
	int Number;
	vector<Component *>Resistors;
	vector<Component *>CurrentSource;
	vector<Component *>VoltageSource;
};

void LoadFile(vector<Node> &Circuit, vector<Component>&Resistors, vector<Component>&VoltageSources, vector<Component>&CurrentSources);

int WhichNodeIsConnected(vector<Node>Circuit, int FirstNode, string name);

void UpgradingTerminals(vector<Node>&Circuit);

void UpgradingVectors(vector<Node>Circuit, vector<Component>&Resistors, vector<Component>&VoltageSources, vector<Component>&CurrentSources);

vector <NewNode>TheCircuitIs(vector<Node>Circuit, vector<Component> &VoltageSources, vector<Component> &CurrentSources, vector<Component>&Resistors);

void ConnectElementsToNode(vector<NewNode>&Circuit, vector<Component> &VoltageSources, vector<Component> &CurrentSources,
	vector<Component>&Resistors, int i, vector<Component>Aux, int SizeOfNodes);

int GetTheIndex(string name, vector<Component> Objects);

bool FirstAppear(string name, vector<Component>Aux);

bool isRef(Node* node);
bool CheckEssential(Node* node);
double CalculateG(Node* node);
double CalculateMutualG(Node* node1, Node* node2);
double CalculateCurrent(Node* node);
MatrixXd BuildMatrixG(vector<Node> nodes);
MatrixXd BuildMatrixI(vector<Node> nodes);
MatrixXd GetMatrixV(MatrixXd G, MatrixXd I);
void BindVoltageValues(vector<Node> nodes, MatrixXd matrixV);
void PerformNodeAnalysis(vector<Node> nodes);
void ConvertVStoCS(Node* node, vector<Node> nodes);
void ConvertCircuit(vector<Node> nodes);
