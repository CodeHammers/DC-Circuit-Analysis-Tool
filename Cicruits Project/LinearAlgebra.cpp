/*#include "Components.h"
//Builds G matrix 1/sumRes for all nodes
MatrixXd BuildMatrixG(vector<Node> nodes) {
	MatrixXd matrixG(nodes.size(), nodes.size());
	int i = 0; int j = 0;
	for (std::vector<Node>::iterator outerIt = nodes.begin(); outerIt != nodes.end(); ++outerIt) {
		for (std::vector<Node>::iterator innerIt = nodes.begin(); innerIt != nodes.end(); ++innerIt) {
			matrixG(i,j)= CalculateMutualG(&(*outerIt), &(*innerIt));
			j++;
		}
		j = 0;
		i++;
	}
	return matrixG;
}
//Builds Currents matrix 

MatrixXd BuildMatrixI(vector<Node> nodes) {
	MatrixXd matrixI(nodes.size(), nodes.size());
	int i = 0; int j = 0;
	for (std::vector<Node>::iterator outerIt = nodes.begin(); outerIt != nodes.end(); ++outerIt) {
		for (std::vector<Node>::iterator innerIt = nodes.begin(); innerIt != nodes.end(); ++innerIt) {
			matrixI(i, j) = CalculateMutualCurrent(&(*outerIt), &(*innerIt));
			j++;
		}
		j = 0;
		i++;
	}
	return matrixI;
}
//Builds the required Voltage matrix
MatrixXd GetMatrixV(MatrixXd G, MatrixXd I) {
	return G.inverse() * I ;
}
*/