#pragma once
#include "Libraries.h"

struct Component
{
	float Magnitude;
	string Label;
	int Terminal1;
	int Terminal2;
	char T1Sign;
	char T2Sign;
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
bool isRef(Node* node);
bool CheckEssential(Node* node);
double CalculateG(Node* node);
double CalculateMutualG(Node* node1, Node* node2);
double CalculateCurrent(Node* node);
MatrixXd BuildMatrixG(vector<Node> &nodes);
MatrixXd BuildMatrixI(vector<Node> &nodes);
MatrixXd GetMatrixV(MatrixXd G, MatrixXd I);
void BindVoltageValues(vector<Node> &nodes, MatrixXd matrixV);
void PerformNodeAnalysis(vector<Node> &nodes);
void ConvertVStoCS(Node* node, vector<Node> &nodes);
void ConvertCircuit(vector<Node> &nodes);
void DeConvertCircuit(vector<Node> &nodes);
void RecoverNode(Node* node, vector<Node> &nodes);