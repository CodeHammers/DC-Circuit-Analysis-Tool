#pragma once
#include"Components.h"
void LoadFile(vector<NewNode> &Circuit, vector<Component>&Resistors, vector<Component>&VoltageSources, vector<Component>&CurrentSources);
int GetTheIndex(string name, vector<Component> Objects);
bool FirstAppear(string name, vector<Component>Resistors, vector<Component>VoltageSources, vector<Component>CurrentSources);
