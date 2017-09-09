#include <cekeikon.h>
#include<stdio.h>
#include <sstream>

using namespace std;

int main(int argc, char** argv)
{ 
	int n = argc-2;
	for(int i = 0; i < n; i++){
		
		if(atoi(argv[1]) == 1){
			stringstream ss;
			ss << argv[i+2] << ".jpg";
			ss.str();
			rename( argv[i+2] , ss.str().c_str() );
		}else if(atoi(argv[1]) == 2){	
			Mat_<COR> foto; 
			le(foto, argv[i+2]);
			cv::resize(foto, foto, cv::Size(32,64));
			stringstream ss;
			ss << argv[i+2];
			ss.str();
			imp(foto, ss.str());
		}else if(atoi(argv[1]) == 3){	
			Mat_<COR> foto; 
			Mat_<GRY> fotoGray;
			le(foto, argv[i+2]);
			cvtColor(foto,fotoGray,CV_BGR2GRAY);
			stringstream ss;
			ss << argv[i+2];
			ss.str();
			imp(fotoGray, ss.str());
		}else if(atoi(argv[1]) == 4){	
			ImgXyr<GRY> foto; 
			le(foto, argv[i+2]);
			ImgXyb<GRY> d(20*foto.rows,20*foto.cols);

			foto.centro(); 
			for (int x=d.minx; x<=d.maxx; x++)
				for (int y=d.miny; y<=d.maxy; y++)
					d(x,y)=foto(x,y);
				imp(d,argv[i+2]);
		}
	}
	return 0;
}