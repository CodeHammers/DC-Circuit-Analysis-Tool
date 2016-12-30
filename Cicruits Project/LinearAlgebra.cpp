#include "Components.h"
//Builds G matrix 1/sumRes for all nodes
MatrixXd BuildMatrixG(vector<Node> &nodes) {
	MatrixXd matrixG(nodes.size(), nodes.size());
	int i = 0; int j = 0;
	for (std::vector<Node>::iterator outerIt = nodes.begin(); outerIt != nodes.end(); ++outerIt) {
		if (!isRef(&(*outerIt))) {
			for (std::vector<Node>::iterator innerIt = nodes.begin(); innerIt != nodes.end(); ++innerIt) {
				if (!isRef(&(*innerIt))) {
					matrixG(i, j) = CalculateMutualG(&(*outerIt), &(*innerIt));
					j++;
				}
			}
			j = 0;
			i++;
		}
	}
	matrixG = -matrixG;
	matrixG.diagonal() = -matrixG.diagonal();
	return matrixG;
}
//Builds Currents matrix 
MatrixXd BuildMatrixI(vector<Node> &nodes) {
	MatrixXd matrixI(nodes.size(), 1);
	int i = 0; 
	for (std::vector<Node>::iterator outerIt = nodes.begin(); outerIt != nodes.end(); ++outerIt) {
		if (!isRef(&(*outerIt))) {
			matrixI(i, 0) = CalculateCurrent(&(*outerIt));
			i++;
		}
	}
	return matrixI;
}
//Builds the required Voltage matrix
MatrixXd GetMatrixV(MatrixXd G, MatrixXd I) {
	return G.inverse() * I ;
}
