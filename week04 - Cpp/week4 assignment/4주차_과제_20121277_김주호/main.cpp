#include "GrowableArray.h"
#include <iostream>
#include <string>
using namespace std;

int main(void) {

	cout << "GrowableArray<int> Test" << endl;
	GrowableArray<int> g(10);
	int i;
	for (i = 0; i < g.length(); i++) g[i] = 2 * i + 3;
	cout << "g(10)"; g.print();
	g[13] = 13;
	g[16] = 16;
	cout << "g(26)"; g.print();


	cout << "GrowableArray<double> Test" << endl;
	GrowableArray<double> dg(10);
	//int i;
	for (i = 0; i < dg.length(); i++) dg[i] = 2.0 * i + 3.14;
	cout << "dg(10)"; dg.print();
	dg[13] = 13.31;
	cout << "dg(26)"; dg.print();


}