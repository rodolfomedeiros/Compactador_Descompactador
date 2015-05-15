struct Header{						//guarda dos os simbolos e frequência existentes no texto.
	char symb;
	unsigned short freq;
};

struct headerArvore{				//Registro para montar a arvore com lista...
	char symb;
	unsigned short freq;
	headerArvore * dir;
	headerArvore * esq;
};

void compactarView();	//View principal de codificar.

bool compactarControl(char* & );	//Controle do codificar.

bool getCaracteresTexto(char* & , unsigned short* & , unsigned short & );	//abre o arquivo e pega todos os caracteres e respectivas frequência.

bool setCaractesresHeader(Header* & , unsigned short* & , unsigned short & );	

headerArvore * criandoArvore(Header* & header, unsigned short & );

bool functionObject(const headerArvore *, const headerArvore *);		//função objeto para o "list.sort();"

void pre_ordem(headerArvore* & pt, char* * stringBinario, char* str);

bool getQuantidadeBits(unsigned short & nbits, char* * stringBinario, Header* & header, unsigned short & alphabetSize);		//pega a quantidade de bits necessaria = nbits.

bool getCodigoBinarioFinal(char* & data, char* * stringBinario, char* & arquivo, int & vetsize);

bool gerarArquivoCompactado(char* & arquivo, unsigned short & alphabetSize, Header* & header, unsigned short & nbits, char* & data, int & vetsize);

bool compactarViewFinal(unsigned short & alphabetSize, Header* & header, int & vetsize, int & tamOriginal, int & tamCodificado, double & tamPorcentagem);