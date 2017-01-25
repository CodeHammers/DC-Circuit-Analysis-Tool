#pragma once
#include "Libraries.h"


struct Component
{
	float Magnitude;
	string Label;
	int Terminal1;
	int Terminal2;
	int T1Sign;
	int T2Sign;
};
struct Node
{
	int Number;
	double voltage;
	bool deprecated;
	bool isRef;
	bool voltageSet;
	vector<Component*>Resistors;
	vector<Component*>CurrentSource;
	vector<Component*>VoltageSource;
};

bool isRef(Node* node);
bool CheckEssential(Node* node);
double CalculateG(Node* node);
double CalculateMutualG(Node* node1, Node* node2);
double CalculateCurrent(Node* node);
MatrixXd BuildMatrixG(vector<Node> &nodes);
MatrixXd BuildMatrixI(vector<Node> &nodes);
MatrixXd GetMatrixV(MatrixXd G, MatrixXd I);
void BindVoltageValues(vector<Node> &nodes, MatrixXd matrixV);
int GetBoundryNode(Component* cmp, Node * node);
void PerformNodeAnalysis(vector<Node> &nodes);
void ConvertVStoCS(Node* &node, vector<Node> &nodes);
void ConvertCircuit(vector<Node> &nodes);
void DeConvertCircuit(vector<Node> &nodes);
void RollBackChangesToNode(Node* &node, vector<Node> &nodes);
void SetRefNode(vector<Node> &nodes);

void LoadCircuit(vector<Node> &nodes,vector<Component*> &components);
void LoadNode(vector<Node> &nodes, vector<Component*> &components, int count);
bool FirstAppeared(string label, vector<Component*> &components, Node &node);
void AddComponentToNode(Node &node, Component* &comp);
int GetActualSize(vector<Node> nodes);

bool CheckForOC(vector<Component*> components);