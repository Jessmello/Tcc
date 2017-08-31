#include <cekeikon.h>
int main(int argc, char** argv)
{ 
	if (argc<3) erro("Erro: BooTrain treino.img boo.xml (ou .yaml)");
	Mat_<FLT> tr; 
	le(tr,argv[1]);
	{ // se tem menos de 20 linhas, copia para dar pelo menos 20 linhas.
		int nl=tr.rows;
		int i=0;
		while (tr.rows<20) {
			tr.push_back(tr.row(i));
			i++;
			if (i>=nl) i=0;
		}
	}
	Mat_<FLT> trent=tr(Rect(0,0,tr.cols-1,tr.rows));
	Mat_<FLT> trsai=tr(Rect(tr.cols-1,0,1,tr.rows));
	CvMat pfeatures=trent;
	CvMat psaidas=trsai;
	printf("Treinando...\n");
	CvBoost ind;
	CvBoostParams param(CvBoost::GENTLE, 200, 0.95, 2, false, 0);
	Mat_<GRY> var_types(tr.cols,1,GRY(CV_VAR_ORDERED));
	var_types(tr.cols-1,0)=CV_VAR_CATEGORICAL;
	CvMat pvar_types=var_types;
	ind.train(&pfeatures,CV_ROW_SAMPLE,&psaidas,0,0,&pvar_types,0,param);
	//ind.train(&pfeatures,CV_ROW_SAMPLE,&psaidas,0,0,&pvar_types,0);
	ind.save(argv[2]);
}