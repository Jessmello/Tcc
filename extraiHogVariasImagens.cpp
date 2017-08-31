#include <cekeikon.h>
int main(int argc, char** argv)
{ 
	if (argc<4) {
		printf("Pedestre: Extrai HOG de imagens 128x64 e grava como .IMG\n");
		printf(" Alimenta tambem a imagem espelhada\n");
		printf("Pedestre pedestre.img -1|+1 imagens128x64*.jpg\n");
		printf(" Se nao existe pedestre.img, cria novo\n");
		printf(" Caso contrario, acrescenta novas linhas em pedestre.img\n");
		printf(" -1: exemplo negativo. +1: exemplo positivo.\n");
		erro("Erro: Numero de argumentos invalido");
	}
	int n=argc-3;
	Mat_<FLT> pedant;
	if (existeArq(argv[1])) {
		le(pedant,argv[1]);
		if (pedant.cols!=1024+1) erro("Erro: pedestre.img deve ter 1025 colunas");
	}
	Mat_<FLT> ped(pedant.rows+2*n,1024+1,0.0);
	for (int l=0; l<pedant.rows; l++)
		for (int c=0; c<pedant.cols; c++)
			ped(l,c)=pedant(l,c);
	int inicio=pedant.rows;
	FLT rotulo; 
	convArg(rotulo,argv[2]);
	int ns=8;
	int nl=16;
	int nc=8;
	Mat_<COR> a;
	for (int i=0; i<n; i++) {
		le(a,argv[i+3]);
		printf("Processando %s\r",argv[i+3]);
		{ 
			HOG hog(a,8,8);
			if (hog.blo.size[0]!=ns || hog.blo.size[1]!=nl || hog.blo.size[2]!=nc)
				erro("Erro: Dimensoes diferentes");
			for (int l=0; l<nl; l++)
				for (int c=0; c<nc; c++) {
					for (int s=0; s<ns; s++)
						ped(2*i+inicio,l*nc*ns+c*ns+s)=hog.blo(s,l,c);
					ped(2*i+inicio,1024)=rotulo;
				}
		}
		{
			flip(a,a,1);
			HOG hog(a,8,8);
			if (hog.blo.size[0]!=ns || hog.blo.size[1]!=nl || hog.blo.size[2]!=nc)
			erro("Erro: Dimensoes diferentes");
			for (int l=0; l<nl; l++)
				for (int c=0; c<nc; c++) {
					for (int s=0; s<ns; s++)
						ped(2*i+1+inicio,l*nc*ns+c*ns+s)=hog.blo(s,l,c);
					ped(2*i+1+inicio,1024)=rotulo;
				}
		}
	}
	printf("\n");
	imp(ped,argv[1]);
}