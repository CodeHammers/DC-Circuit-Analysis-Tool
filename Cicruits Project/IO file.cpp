#include "Components.h"

void LoadCircuit(vector<Node> &nodes,vector<Component*> &components) {
	string ToNextNode;
	int count = 1;
	cout << "type end to start a new node" << endl;
	do {
		cout << "Node #" << count << endl;
		LoadNode(nodes, components, count);
		count++;
		cin >> ToNextNode;
	} while (ToNextNode == "Y");

}
void LoadNode(vector<Node> &nodes, vector<Component*> &components, int count) {
	string label;
	Node node; 
	node.deprecated = false;
	node.voltageSet = false;
	node.isRef = false;
	node.Number = count - 1;
	cin >> label;
	while (label != "end") {
		bool flag = FirstAppeared(label, components, node);
		if (flag) {
			Component* comp = new Component();
			comp->Label = label;
			cin >> comp->Magnitude;
			comp->Terminal1 = node.Number;
			comp->Terminal2 = -1;
			if (comp->Magnitude < 0) {
				comp->Magnitude *= -1;
				comp->T1Sign = -1;
				comp->T2Sign = 1;
			}
			else {
				comp->T1Sign = 1;
				comp->T2Sign = -1;
			}
			components.push_back(comp);
			AddComponentToNode(node, comp);
		}
		cin >> label;
	}
	nodes.push_back(node);
}

bool FirstAppeared(string label, vector<Component*> &components, Node &node) {
	float knownValue;
	for (int i = 0; i < components.size(); i++) {
		if (components[i]->Label == label) {
			components[i]->Terminal2 = node.Number;
			Component * comp = components[i];
			cin >> knownValue;
			if (knownValue < 0) 
				comp->T2Sign = -1;	
			else
				comp->T2Sign = 1;
			AddComponentToNode(node, components[i]);
			return false;
		}
	}
	return true;
}

void AddComponentToNode(Node &node, Component* &comp) {
	switch (comp->Label[0])
	{
	case 'R':
		node.Resistors.push_back(comp);
		break;
	case 'J':
		node.CurrentSource.push_back(comp);
		break;
	case 'E':
		node.VoltageSource.push_back(comp);
		break;
	default:
		break;
	}
}