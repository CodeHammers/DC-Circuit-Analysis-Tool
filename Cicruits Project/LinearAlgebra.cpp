#include "Components.h"
//Builds G matrix 1/sumRes for all nodes
MatrixXd BuildMatrixG(vector<Node> &nodes) {
	int ActualSize = GetActualSize(nodes);
	MatrixXd matrixG(ActualSize, ActualSize);//Bug: Actual size = node.size - DeprecatedCount		Number	1	int
	int CarryForDeprecatedOnI = 0;
	int CarryForDeprecatedOnJ = 0;
	for (int i = 0; i < nodes.size(); i++) {
		Node RowNode = nodes[i];
		if (!RowNode.deprecated) {
			for (int j = 0; j < nodes.size(); j++) {
				Node OnRowNode = nodes[j];
				if (!OnRowNode.deprecated) {
					matrixG(i + CarryForDeprecatedOnI, j + CarryForDeprecatedOnJ) = CalculateMutualG(&RowNode, &OnRowNode);
				}
				else {
					CarryForDeprecatedOnJ--;
				}
			}
			CarryForDeprecatedOnJ = 0;
		}
		else {
			CarryForDeprecatedOnI--;
		}
	}
	matrixG = -matrixG;
	matrixG.diagonal() = -matrixG.diagonal();
	//cout << endl << endl;
	/*cout << " Matrix G" << endl;
	cout << matrixG;*/
	return matrixG;
}
//Builds Currents matrix 
MatrixXd BuildMatrixI(vector<Node> &nodes) {
	int ActualSize = GetActualSize(nodes);
	MatrixXd matrixI(ActualSize, 1);
	int CarryForDeprecated = 0;
	for (int i = 0; i < nodes.size(); i++)
		if (!nodes[i].deprecated)
			matrixI(i + CarryForDeprecated, 0) = CalculateCurrent(&nodes[i]);
		else
			CarryForDeprecated--;
	//cout << endl << endl;
	/*cout << " Matrix I" << endl;
	cout << matrixI;*/
	return matrixI;
}
MatrixXd BuildMatrixI(vector<Node> &nodes,string label) {
	if (label[0] == 'E')
		label = "J" + label;
	int ActualSize = GetActualSize(nodes);
	MatrixXd matrixI(ActualSize, 1);
	int CarryForDeprecated = 0;
	for (int i = 0; i < nodes.size(); i++)
		if (!nodes[i].deprecated)
			matrixI(i + CarryForDeprecated, 0) = CalculateCurrent(&nodes[i],label);
		else
			CarryForDeprecated--;
	//cout << endl << endl;
	/*cout << " Matrix I" << endl;
	cout << matrixI;*/
	return matrixI;
}
//Builds the required Voltage matrix
MatrixXd GetMatrixV(MatrixXd G, MatrixXd I) {
	return G.inverse() * I;
}