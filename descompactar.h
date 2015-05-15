struct HeaderD{						//guarda dos os simbolos e frequência existentes no texto.
	char symb;
	unsigned short freq;
};

struct headerArvoreD{				//Registro para montar a arvore com lista...
	char symb;
	unsigned short freq;
	headerArvoreD * dir;
	headerArvoreD * esq;
};

void descompactarView();	//View principal de codificar.

bool descompactarControl(char* & arquivo);	//Controle do codificar.

bool getDadosArquivo(char* & arquivo, int & alphabetSize, HeaderD* & header, int & nbits, char* & data, int & vetsize);

headerArvoreD * criandoArvore(HeaderD* & header, int &);

bool functionObject(const headerArvoreD *, const headerArvoreD *);

void pre_ordem(headerArvoreD* & pt, char* * stringBinario, char* str);

bool identificandoBits(char* * stringBinario, char* & data, int & vetsize, int & nbits);
