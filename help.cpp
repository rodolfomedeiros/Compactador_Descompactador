#include <iostream>

using namespace std;

void tracosComEspaco(char ch, int quant){
	
	cout << endl;
	
	for (int i = 1; i <= quant; i++){
		cout << ch << " ";
	}

	cout << endl;
}

void tracosSemEspaco(char ch, int quant){
	
	cout << endl;

	for (int i = 1; i <= quant; i++){
		cout << ch;
	}

	cout << endl;
}