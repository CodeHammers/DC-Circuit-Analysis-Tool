//main function goes here
#include <iostream>
#include "armadillo"
using namespace arma;
using namespace std;

int main() {
	//example
	mat x;
	x << 1 << 2 << endr << 3 << 4;
	cout << x[0,1];
	cin.get();
}