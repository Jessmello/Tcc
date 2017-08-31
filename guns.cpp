#include <cekeikon.h>
#include <string>

Mat_<FLT> extraiHog(Mat_<COR> a){
	int ns=8;
	int nl=16;
	int nc=8;
	Mat_<FLT> ped(2,1024+1,0.0);
	int inicio=ped.rows;
	{ 
		HOG hog(a,8,8);
			if (hog.blo.size[0]!=ns || hog.blo.size[1]!=nl || hog.blo.size[2]!=nc)
				erro("Erro: Dimensoes diferentes");
			for (int l=0; l<nl; l++)
				for (int c=0; c<nc; c++) {
					for (int s=0; s<ns; s++)
						ped(0,l*nc*ns+c*ns+s)=hog.blo(s,l,c);
					ped(0,1024)=1;
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
					ped(1,l*nc*ns+c*ns+s)=hog.blo(s,l,c);
				ped(1,1024)=1;
			}
	}
	return ped;
}

int main(int argc, char** argv)
{ 
	if (argc<2) erro("Erro: BooPred boo.xml teste.img");
	CvBoost ind;
	ind.load(argv[1]);
	
	float p,r= +1;
	
	printf("Classificando...\n");
	
	VideoCapture capture(argv[2]);
	//VideoCapture capture(0);
	//capture.set(3,64);
	//capture.set(4,128);

	
	
	if (!capture.isOpened())
		throw "Error when reading steam_avi";
	
	
	namedWindow("teste", 1);
	int cont=0;
	while (true)
	{
		Mat_<COR> frame;
		
		int weak_count=ind.get_params().weak_count;
		Mat_<FLT> weak_responses(1,weak_count);
		CvMat pweak_responses=weak_responses;
		capture >> frame;
		
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}
		
		
		//if (cont % 3 == 0) {
			
			//imwrite("image.jpeg", frame2);
			//le(frame2,"image.jpeg");
			Mat_<COR> frame2(frame, Rect(0,0,320,640));
			cv::resize(frame2, frame2, cv::Size(64,128));
			
			Mat_<FLT> te = extraiHog(frame2);
			Mat_<FLT> query(1,te.cols-1);
			CvMat pquery=query;
			for (int l=0; l<te.rows; l++) {
				for (int c=0; c<query.cols; c++)
					query(0,c)=te(l,c);
				p=ind.predict(&pquery,0,&pweak_responses);
				if(p==1){
					std::string nome = std::string("image");
					nome += std::to_string(cont);
					cont++;
					nome += ".ppm";
					imwrite(nome, frame2);
					printf("achou arma");
				}
				
			}
			
			Mat_<COR> frame3(frame, Rect(320,0,320,640));
			cv::resize(frame3, frame3, cv::Size(64,128));
			
			te = extraiHog(frame3);
			query(1,te.cols-1);
			pquery=query;
			for (int l=0; l<te.rows; l++) {
				for (int c=0; c<query.cols; c++)
					query(0,c)=te(l,c);
				p=ind.predict(&pquery,0,&pweak_responses);
				if(p==1){
					std::string nome = std::string("image");
					nome += std::to_string(cont);
					cont++;
					nome += ".ppm";
					imwrite(nome, frame3);
					printf("eh Pedestre: %.f\n", p);
				}
				
			}
			
			Mat_<COR> frame4(frame, Rect(640,0,320,640));
			cv::resize(frame4, frame4, cv::Size(64,128));
			
			te = extraiHog(frame4);
			query(1,te.cols-1);
			pquery=query;
			for (int l=0; l<te.rows; l++) {
				for (int c=0; c<query.cols; c++)
					query(0,c)=te(l,c);
				p=ind.predict(&pquery,0,&pweak_responses);
				if(p==1){
					std::string nome = std::string("image");
					nome += std::to_string(cont);
					cont++;
					nome += ".ppm";
					imwrite(nome, frame4);
					printf("eh Pedestre: %.f\n", p);
				}
				
			}
			
			Mat_<COR> frame5(frame, Rect(960,0,320,640));
			cv::resize(frame5, frame5, cv::Size(64,128));
			
			te = extraiHog(frame5);
			query(1,te.cols-1);
			pquery=query;
			for (int l=0; l<te.rows; l++) {
				for (int c=0; c<query.cols; c++)
					query(0,c)=te(l,c);
				p=ind.predict(&pquery,0,&pweak_responses);
				if(p==1){
					std::string nome = std::string("image");
					nome += std::to_string(cont);
					cont++;
					nome += ".ppm";
					imwrite(nome, frame5);
					printf("eh Pedestre: %.f\n", p);
				}
				
			}
		//}
		imshow("teste", frame);
		

		char c = (char)waitKey(30);
		if (c == 27) { break; } // escape
	}
	return 0;
	
}


