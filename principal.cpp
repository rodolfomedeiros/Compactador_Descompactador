#include <iostream>
#include "help.h"
#include "compactar.h"
#include "descompactar.h"

using namespace std;

void principalView(){
	char opcao[100];

	do{
		system("cls");

		tracosSemEspaco('-', 60);

		cout << "          COMPACTADOR E DESCOMPACTADOR";

		tracosSemEspaco('-', 60);
		
		cout << "(1) - Compactar" << endl;						//opções
		cout << "(2) - Descompactar" << endl;					//
		cout << "(0) - Sair" << endl << endl;					//

		cout << "Escolha uma opcao: ";
		
		cin >> opcao;

		if (strcmp(opcao, "1") == 0)	compactarView();		//chama a função compactar.

		if (strcmp(opcao, "2") == 0)	descompactarView();		//chama a função descompactar.
		
	} while (!(strcmp(opcao, "0") == 0));		//sai do sistema

}

