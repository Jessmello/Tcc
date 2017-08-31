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
		}else{			
			Mat_<COR> foto; 
			le(foto, argv[i+2]);
			cv::resize(foto, foto, cv::Size(192,384));
			stringstream ss;
			ss << argv[i+2] << "_r.jpg";
			ss.str();
			imp(foto, ss.str());
		}
		
		
		
	}
	return 0;
}