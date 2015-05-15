#include <iostream>
#include <fstream>
#include <list>
#include "help.h"
#include "descompactar.h"

using namespace std;

void descompactarView(){
	char* arquivo = new char[100];

	cout << endl;
	tracosSemEspaco('-', 60);
	cout << endl;
	cout << "Descompactar o Texto:" << endl;
	cout << "Informe o Nome do Arquivo(.huf): "; cin >> arquivo;

	if (!(descompactarControl(arquivo)))	//chama a função de controle do descompactador, e passa como parâmetro o nome do arquivo
	{
		cout << "A DESCOMPACTAÇÃO FALHOU" << endl;
	}

	delete[] arquivo;

	cout << endl;
	system("pause");
}

bool descompactarControl(char* & arquivo){
	int alphabetSize = 0;
	int nbits = 0;
	int vetsize = 0;
	HeaderD* header = nullptr;
	char* data = nullptr;

	if (!getDadosArquivo(arquivo, alphabetSize, header, nbits, data, vetsize)){
		return false;
	}

	headerArvoreD * headerRaiz = criandoArvore(header, alphabetSize);		//retorna a raiz da arvore de huffman.

	char* stringBinario[256] = { nullptr };		//vetor de Strings de Binarios.

	pre_ordem(headerRaiz, stringBinario, "");	//pecorrer a arvore em pre-ordem	, gerando a "String de binarios"

	identificandoBits(stringBinario, data, vetsize, nbits);

	return true;
}

bool getDadosArquivo(char* & arquivo, int & alphabetSize, HeaderD* & header, int & nbits, char* & data, int & vetsize){
	ifstream fin;
	fin.open(arquivo, ios_base::in | ios_base::binary);

	if (!fin.is_open()){
		cout << "Arquivo " << arquivo << " não abriu!!!" << endl;
		return false;
	}

	// o número de símbolos diferentes que aparecem no texto
	fin.read((char*) &alphabetSize, sizeof(unsigned short));

	// aloca espaço para a informação do cabeçalho
	header = new HeaderD[alphabetSize];
	fin.read((char*) header, sizeof(HeaderD) * alphabetSize);

	// lê o número de bits armazenados no arquivo e calcula tamanho do vetor
	fin.read((char*) &nbits, sizeof(unsigned short));

	vetsize = ((nbits % 8) == 0) ? (nbits / 8) : ((nbits / 8) + 1);

	// aloca espaço para os dados
	data = new char[vetsize];
	fin.read((char*) data, sizeof(char) * vetsize);
	
	fin.close();
	return true;
}

headerArvoreD * criandoArvore(HeaderD* & header, int & alphabetSize){
	list<headerArvoreD *> lista;			//cria uma lista de ponteiro para "headerArvoreD"

	for (unsigned short i = 0; i < alphabetSize; i++){			//adiciona todo o cabecalho para lista
		headerArvoreD * headerarvore = new headerArvoreD;		//
		headerarvore->symb = header[i].symb;						//
		headerarvore->freq = header[i].freq;						//
		headerarvore->dir = nullptr;								//
		headerarvore->esq = nullptr;								//
		lista.push_back(headerarvore);							//
	}

	lista.sort(functionObject);			//ordena a lista do menor para o maior, utilizando a funcao objeto

	list<headerArvoreD *>::iterator li;
	headerArvoreD * no1;
	headerArvoreD * no2;

	li = lista.begin();
	while (li != lista.end()){
		no1 = *li;
		li++;
		if (li != lista.end()){								//gera novo nó, com dois nós existentes, depois retorna para lista.
			no2 = *li;										//
			headerArvoreD * novo = new headerArvoreD;		//
			novo->esq = no1;									//
			novo->dir = no2;									//
			novo->freq = no1->freq + no2->freq;				//
			novo->symb = NULL;								//

			lista.remove(no1);								//
			lista.remove(no2);								//
			lista.push_back(novo);							//
			lista.sort(functionObject);						//

			li = lista.begin();
		}
	}

	li = lista.begin();

	return *li;
}

bool functionObject(const headerArvoreD * a, const headerArvoreD * b){
	return a->freq < b->freq;
}

void pre_ordem(headerArvoreD* & pt, char* * stringBinario, char* str){
	if (pt->symb != NULL){
		unsigned short i;
		stringBinario[pt->symb] = new char[strlen(str)];
		for (i = 0; i < strlen(str); i++){
			stringBinario[pt->symb][i] = str[i];
		}
		stringBinario[pt->symb][i] = '\0';
	}
	if (pt->esq != nullptr){
		unsigned short i;
		char* novo = new char[strlen(str) + 2];
		for (i = 0; i < strlen(str); i++){
			novo[i] = str[i];
		}
		novo[i] = '0';
		novo[i + 1] = '\0';
		pre_ordem(pt->esq, stringBinario, novo);
		delete[] novo;
	}
	if (pt->dir != nullptr){
		unsigned short i;
		char* novo1 = new char[strlen(str) + 2];
		for (i = 0; i < strlen(str); i++){
			novo1[i] = str[i];
		}
		novo1[i] = '1';
		novo1[i + 1] = '\0';
		pre_ordem(pt->dir, stringBinario, novo1);
		delete[] novo1;
	}
}

bool identificandoBits(char* * stringBinario, char* & data, int & vetsize, int & nbits){
	int posicao = 0;
	char* gerandoString = new char[1000];
	char valueChar;
	int passou;

	tracosSemEspaco('-', 60);
	
	for (int i = 0; i < vetsize; i++){
		valueChar = data[i];
		if (i == (vetsize - 1)){
			passou = nbits % 8 == 0 ? 0 : (8 - ((vetsize * 8) - nbits));		//atribui a o que passou no ultimo caractere, ou seja, o que ainda falta de bits ára gerar um simbolo.
		}
		for (int j = 0; j < 8; j++){
			if ((i == (vetsize - 1)) && (passou == j)){			//tratando o ultimo caractere.
				break;
			}
			gerandoString[posicao] = valueChar & 128 ? '1' : '0';	//atribue a posição, pega da esquerda para direita. 128(10000000).
			posicao++;
			gerandoString[posicao] = '\0';

			for (int k = 0; k < 256; k++){
				if (stringBinario[k] != nullptr){							//Verifico se as strings são igual, se sim, print na tela.
					if (strcmp(stringBinario[k], gerandoString) == 0){		//
						cout << (char) k;									//
						posicao = 0;										//
						break;												//
					}														//
				}															//
			}
			valueChar = valueChar << 1;		//desloca uma posição para esquerda.
		}
	}

	tracosSemEspaco('-', 60);

	return false;
}