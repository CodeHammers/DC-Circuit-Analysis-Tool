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
	vector<Component>Resistors;
	vector<Component>CurrentSource;
	vector<Component>VoltageSource;
};

bool checkEssential(Node* node);