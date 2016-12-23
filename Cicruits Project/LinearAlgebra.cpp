#include "Components.h"
//Builds G matrix 1/sumRes for all nodes

mat BuildMatrixG(vector<Node> nodes) {
	mat matrixG;
	for (std::vector<Node>::iterator outerIt = nodes.begin(); outerIt != nodes.end(); ++outerIt) {
		for (std::vector<Node>::iterator innerIt = nodes.begin(); innerIt != nodes.end(); ++innerIt) {
			matrixG << CalculateMutualG(&(*outerIt),&(*innerIt));
		}
		matrixG << endr;
	}
	return matrixG;
}
//Builds Currents matrix 
mat BuildMatrixI(vector<Node> nodes) {
	mat matrixI;
	for (std::vector<Node>::iterator outerIt = nodes.begin(); outerIt != nodes.end(); ++outerIt) {
		for (std::vector<Node>::iterator innerIt = nodes.begin(); innerIt != nodes.end(); ++innerIt) {
			matrixI << CalculateMutualCurrent(&(*outerIt), &(*innerIt));
		}
		matrixI << endr;
	}
	return matrixI;
}
//Builds the required Voltage matrix
//mat GetMatrixV(mat G, mat I) {
//	return G * I;
//}