#include <iostream>
#include <fstream>
#include <list>
#include <iomanip>
#include "help.h"
#include "compactar.h"

using namespace std;

void compactarView(){
	char* arquivo = new char[100];  // aloca espaço para o nome do arquivo.

	cout << endl;
	tracosSemEspaco('-', 60);
	cout << endl;
	cout << "Compactar o Texto;" << endl;               
	cout << "Informe o nome do Arquivo(.txt): "; cin >> arquivo;			//recebe o nome do arquivo.

	if (!(compactarControl(arquivo)))	//chama a função de controle do compactar, e passa como parâmetro o nome do arquivo
	{
		cout << endl <<"A COMPACTAÇÃO FALHOU"<< endl;
	}

	delete[] arquivo;

	cout << endl;
	system("pause");
}

bool compactarControl(char* & arquivo){
	unsigned short * todosCaracteres = new unsigned short[256]{0};			//Cria um vetor com os espações para todos os tipos de caracteres
																			//disponiveis na tabela ASCII.					
	unsigned short alphabetSize = 0;				  //Contar a quantidade de caracteres validos.

	if (!(getCaracteresTexto(arquivo, todosCaracteres, alphabetSize))){		//abre o arquivo .txt, pega todo o alfabeto utilizado no texto. Retorna "true" se tudo deu certo.
		return false;
	}

	if (alphabetSize == 0){
		tracosSemEspaco('-', 60);
		cout << endl;
		cout << "      O ARQUIVO ESTA VAZIO!!!" << endl;
		tracosSemEspaco('-', 60);
		cout << endl;
		return false;
	}

	Header * header = new Header[alphabetSize];

	if (!(setCaractesresHeader(header, todosCaracteres, alphabetSize))){	//atribui o simbolo e frequência ao header
		return false;
	}

	delete[] todosCaracteres;

	headerArvore * headerRaiz = criandoArvore(header, alphabetSize);		//retorna a raiz da arvore de huffman.

	char* stringBinario[256] = { nullptr };		//vetor de Strings de Binario, onde estão todas a minhas simbolos codificadas.

	pre_ordem(headerRaiz, stringBinario, "");	//pecorrer a arvore em pre-ordem	

	unsigned short nbits = 0;

	if (!(getQuantidadeBits(nbits, stringBinario, header, alphabetSize))){	//pega quantidade de bits total utilizada
		return false;
	}

	int vetsize = ((nbits % 8) == 0) ? (nbits/8) : ((nbits/8) + 1);	//calculo do tamanho do vetor de caractere

	char* data = new char[vetsize];	//cria o vetor de caracteres

	for (int i = 0; i < vetsize; i++){	//deixa todos os bits == 0
		data[i] = data[i] & 0;
	}

	if (!(getCodigoBinarioFinal(data, stringBinario, arquivo, vetsize))){	//codifica todos os simbolos em bits por ordem...
		return false;
	}
	
	//Pegando Tamanho do Arquivo Original.
	ifstream fin;
	fin.open(arquivo);
	fin.seekg(0, fin.end);
	int tamOriginal = fin.tellg();
	fin.close();

	if (!(gerarArquivoCompactado(arquivo, alphabetSize, header, nbits, data, vetsize))){	//gera o arquivo codificado
		return false;
	}

	//Pegando o tamanho do Arquivo Codificado.
	int tamCodificado = 0;
	tamCodificado = sizeof(alphabetSize) + (sizeof(Header) * alphabetSize) + sizeof(nbits) + vetsize;
	
	//Pegando a porcentagem.
	double tamPorcentagem = ((double) tamCodificado / (double) tamOriginal) * 100;

	//Mostra os resultados
	compactarViewFinal(alphabetSize, header, vetsize, tamOriginal, tamCodificado, tamPorcentagem);

	delete[] header;
	delete[] data;

	return true;
}

bool getCaracteresTexto(char* & arquivo, unsigned short* & todosCaracteres, unsigned short & alphabetSize){
	ifstream fin;
	fin.open(arquivo);	//Abre o arquivo.

	if (!fin.is_open()){	//Verifica se foi aberto.

		cout << "A abertura do arquivo " << arquivo << " falhou!" << endl;
		cout << "Sera retornado ao menu principal." << endl;

		return false;
	}

	char ch;

	ch = fin.get();

	while (fin.good()){
		if (todosCaracteres[ch] == 0){				//usa como o indice o proprio caractere
			alphabetSize += 1;
		}
		todosCaracteres[ch] += 1;					//em cada indice, vai somando a frequencia
		ch = fin.get();
	}

	if (fin.eof()){			//vefifica se chegou no final de arquivo
		fin.close();		//fecha arquivo
		return true;		//retorna true
	}

	return false;
}

bool setCaractesresHeader(Header* & header, unsigned short* & todosCaracteres, unsigned short	& alphabetSize){
	
	unsigned short quantHeader = 0;
	
	for (unsigned short i = 0; i < 256; i++){				
		if (todosCaracteres[i] > 0){							//pecorrer todos os caracteres
			header[quantHeader].symb = (char) i;				//adiciona cada caractere e sua frequencia ao cabeçalho
			header[quantHeader].freq = todosCaracteres[i];
			quantHeader++;
		}
	}

	if (quantHeader == alphabetSize){		
		return true;					
	}
	else return false;
	
}

headerArvore * criandoArvore(Header* & header, unsigned short & alphabetSize){
	list<headerArvore *> lista;			//cria uma lista de ponteiro para "headerArvore"

	for (unsigned short i = 0; i < alphabetSize; i++){			//adiciona todo o cabecalho para lista
		headerArvore * headerarvore = new headerArvore;			//
		headerarvore->symb = header[i].symb;						//
		headerarvore->freq = header[i].freq;						//
		headerarvore->dir = nullptr;								//
		headerarvore->esq = nullptr;								//
		lista.push_back(headerarvore);							//
	}

	lista.sort(functionObject);			//ordena a lista do menor para o maior, utilizando a funcao objeto

	list<headerArvore *>::iterator li;
	headerArvore * no1;
	headerArvore * no2;

	li = lista.begin();
	while (li != lista.end()){
		no1 = *li;
		li++;
		if (li != lista.end()){
			no2 = *li;
			headerArvore * novo = new headerArvore;
			novo->esq = no1;
			novo->dir = no2;
			novo->freq = no1->freq + no2->freq;
			novo->symb = NULL;

			lista.remove(no1);
			lista.remove(no2);
			lista.push_back(novo);
			lista.sort(functionObject);

			li = lista.begin();
		}
	}

	li = lista.begin();

	return *li;
}

bool functionObject(const headerArvore * a, const headerArvore * b){
	return a->freq < b->freq;
}

void pre_ordem(headerArvore* & pt, char* * stringBinario, char* str){
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
		delete [] novo;
	}
	if (pt->dir != nullptr){
		unsigned short i;
		char* novo1 = new char[strlen(str)+2];
		for (i = 0; i < strlen(str); i++){
			novo1[i] = str[i];
		}
		novo1[i] = '1';
		novo1[i+1] = '\0';
		pre_ordem(pt->dir, stringBinario, novo1);
		delete [] novo1;
	}
}

bool getQuantidadeBits(unsigned short & nbits, char* * stringBinario, Header* & header, unsigned short & alphabetSize){
	unsigned short i;

	for (i = 0; i < alphabetSize; i++){
		nbits = (strlen(stringBinario[header[i].symb]) * header[i].freq) + nbits;
	}
	if (i == alphabetSize){
		return true;
	}
	return false;
}

bool getCodigoBinarioFinal(char* & data, char* * stringBinario, char* & arquivo, int & vetsize){
	ifstream fin;

	fin.open(arquivo);

	if (!fin.is_open()){	//Verifica se foi aberto.

		cout << "A abertura do arquivo " << arquivo << " falhou!" << endl;
		cout << "Sera retornado ao menu principal." << endl;

		return false;
	}

	char ch;

	unsigned short dataTam = 0;
	unsigned short contador = 0;

	ch = fin.get();

	while (fin.good()){
		for (int p = 0; p < strlen(stringBinario[ch]); p++){
			if (stringBinario[ch][p] == '1'){
				data[dataTam] = data[dataTam] | 1;
			}
			if (contador < 7){
				data[dataTam] = data[dataTam] << 1;
			}
			contador++;
			if ((contador == 8) && (dataTam != (vetsize-1))){
				contador = 0;
				dataTam++;
			}
		}
		if (dataTam == (vetsize - 1)){
			data[dataTam] = data[dataTam] << (7 - contador);
		}
		ch = fin.get();
	}

	if (fin.eof()){
		fin.close();
		return true;
	}

	return false;
}

bool gerarArquivoCompactado(char* & arquivo, unsigned short & alphabetSize, Header* & header, unsigned short & nbits, char* & data, int & vetsize){
	unsigned short tam = strlen(arquivo);

	arquivo[tam-3] = 'h';
	arquivo[tam-2] = 'u';
	arquivo[tam-1] = 'f';

	ofstream fout;

	fout.open(arquivo, ios_base::out | ios_base::binary);
	
	fout.write((char*) &alphabetSize, sizeof(unsigned short));
	fout.write((char*) header, sizeof(Header) * alphabetSize);
	fout.write((char*) &nbits, sizeof(unsigned short));
	fout.write((char*) data, sizeof(char) * vetsize);
	
	fout.close();

	return true;
}

bool compactarViewFinal(unsigned short & alphabetSize, Header* & header, int & vetsize, int & tamOriginal, int & tamCodificado, double & tamPorcentagem){
	cout << endl << endl << endl;
	tracosSemEspaco('-', 60);
	cout << "          TEXTO CODIFICADO COM SUCESSO!" << endl;
	tracosSemEspaco('-', 60);

	cout << setw(40) << left << "Tamanho Alfabeto:" << alphabetSize << " simbolos"<< endl;
	cout << setw(40) << left << "Tamanho do Alfabeto Codificado:" << sizeof(Header) * alphabetSize << " bytes" << endl;
	cout << setw(40) << left << "Tamanho dos Dados Codificado:" << vetsize << " bytes" << endl;
	tracosComEspaco('-', 30);
	cout << setw(40) << left << "Tamanho do Arquivo Original:" << tamOriginal << " bytes" << endl;
	cout << setw(40) << left << "Tamanho do Arquivo Codificado:" << tamCodificado << " bytes" << endl;
	tracosComEspaco('-',30);
	cout << "O Arquivo Compactado e " << setprecision(4) << tamPorcentagem << "% do Arquivo Original" << endl<< endl << endl;


	return true;
}