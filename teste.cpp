//https://medium.com/@ageitgey/machine-learning-is-fun-part-4-modern-face-recognition-with-deep-learning-c3cffc121d78
//http://www.cv-foundation.org/openaccess/content_cvpr_2015/app/1A_089.pdf
//http://www.csc.kth.se/~vahidk/papers/KazemiCVPR14.pdf
//https://medium.com/@ageitgey/machine-learning-is-fun-part-3-deep-learning-and-convolutional-neural-networks-f40359318721
//https://pdfs.semanticscholar.org/efb1/dbe24de0dca916871566e3b36f3a15757809.pdf
#define DNN_USE_IMAGE_API
#include <cekeikon.h>
#include "tiny_dnn/tiny_dnn.h"
#include <vector>
#include <chrono>
#include <string>

#define EPSILON 0.0002

#define COUT(s) cout << #s " : " << s << "\n";

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;
using namespace std;

#define CROP_X_ 168
#define CROP_Y_ 154
#define OFFSET_X_ 34
#define OFFSET_Y_ 20

#define INPUT_SIZE_X (CROP_X_ - OFFSET_X_)
#define INPUT_SIZE_Y (CROP_Y_ - OFFSET_Y_)
#define OUTPUT_SIZE 2
#define HIDDEN_LAYER1 550 //era 500
#define HIDDEN_LAYER2 250
#define TRAIN_EPOCHS 5
#define BATCH_SIZE 3
#define WINDOW_HOG 24


typedef vector<vector<Mat_<GRY>>> db;

static db db_IMG_train;
static db db_IMG_test;
static double * norm_max;

//Funcao para trocar um caracter s da string str por um outro r
inline string replaceChar(string str, char s, char r){
    for(auto &c : str) if(c == s) c = r;
    return str;
}

template<typename T> inline Mat_<T> conv2Dto1D(Mat_<T> img){
  Mat_<T> ret(1,img.total());
  uint64_t pixel = 0;
    for(int i = 0; i < img.rows; i++){
       for(int j = 0; j < img.cols; j++, pixel++){
        ret(0,pixel) = img(i,j);
      }
    }
    imp(ret,"debug.png");
  return ret;  
}

template<typename T> inline Mat_<T> crop_img(Mat_<T> img, int max_x, int max_y, int offset_x = 0, int offset_y = 0){
  
  Mat_<T> cropped(max_x - offset_x, max_y - offset_y, 1);

  for(int y = 0; offset_y+y < max_y; y++){
    for(int x = 0; offset_x+x < max_x; x++){
      cropped(x,y) = img(offset_x+x,offset_y+y);
    }
  }

  return cropped;
  
}

vector<vector<string>> ler(string arq){
	vector<vector<string>> ret;

	ifstream arquivo(arq.c_str());

	int index_read,index_atual;
	string file_name_buffer,index_buffer;

	vector<string> read;
	do{  
		getline(arquivo,file_name_buffer,';');
		getline(arquivo,index_buffer);
		stringstream ss;

		ss << index_buffer;
		ss >> index_read;

		if(file_name_buffer[0] == 0) continue;

		//cria outro vetor de vector<string>
		if(!(ret.size() == 0 && read.size() == 0)/*se nao for a primeira vez que roda*/
			&&
			index_atual != index_read /*e os valores forem diferentes*/){
	    	ret.push_back(read);
	    	read.clear();
		}

	    index_atual = index_read;
    //necessita trocar por conta do linux. No windows, tanto faz
		read.push_back(replaceChar(file_name_buffer,'\\','/'));

	}while(file_name_buffer[0] != 0);

	//Salva os ultimos valores
	ret.push_back(read);

	return ret;

}

int argMax(vec_t a) {
  auto maximo=a[0]; int indmax=0;
  for (int i=1; i<a.size(); i++)
    if (a[i]>maximo) {
      maximo=a[i]; indmax=i;
    }
  return indmax;
}

float argvMax(vec_t a) {
  auto maximo=a[0];
  for (int i=1; i<a.size(); i++)
    if (a[i]>maximo)
      maximo=a[i];

  return maximo;
}

void normalizar(vector<vec_t> &v){
  if(!norm_max){
    norm_max = new double(v[0][0]);
    auto max_vec = *norm_max = argvMax(v[0]);

    for(auto &vect : v){
      max_vec = argvMax(vect);
      if(*norm_max < max_vec) *norm_max=max_vec;
    }
  }
  for(auto &vect : v)
   for(auto &c : vect)
    c /= *norm_max; 
}


int main(int argc, char* argv[]){

  network<sequential> net;



  vector<label_t> label;

  vector<vec_t> images;  
//grayscale
 /* tiny_dnn::image<> bgr_img("C:\\Users\\Jess\\Desktop\\aa\\y\\1.jpg", image_type::grayscale);
  vec_t vec = bgr_img.to_vec();
  tiny_dnn::image<> bgr_img2("C:\\Users\\Jess\\Desktop\\aa\\y\\2.jpg", image_type::grayscale);
  vec_t vec2 = bgr_img2.to_vec();
  tiny_dnn::image<> bgr_img3("C:\\Users\\Jess\\Desktop\\aa\\y\\3.jpg", image_type::grayscale);
  vec_t vec3 = bgr_img3.to_vec();
  tiny_dnn::image<> bgr_img4("C:\\Users\\Jess\\Desktop\\aa\\y\\4.jpg", image_type::grayscale);
  vec_t vec4 = bgr_img4.to_vec();
  tiny_dnn::image<> bgr_img5("C:\\Users\\Jess\\Desktop\\aa\\y\\5.jpg", image_type::grayscale);
  vec_t vec5 = bgr_img5.to_vec();
  tiny_dnn::image<> bgr_img6("C:\\Users\\Jess\\Desktop\\aa\\y\\6.jpg", image_type::grayscale);
  vec_t vec6 = bgr_img6.to_vec();
  tiny_dnn::image<> bgr_img7("C:\\Users\\Jess\\Desktop\\aa\\y\\7.jpg", image_type::grayscale);
  vec_t vec7 = bgr_img7.to_vec();
  tiny_dnn::image<> bgr_img8("C:\\Users\\Jess\\Desktop\\aa\\y\\8.jpg", image_type::grayscale);
  vec_t vec8 = bgr_img8.to_vec();
  tiny_dnn::image<> bgr_img9("C:\\Users\\Jess\\Desktop\\aa\\y\\9.jpg", image_type::grayscale);
  vec_t vec9 = bgr_img9.to_vec();
  tiny_dnn::image<> bgr_img10("C:\\Users\\Jess\\Desktop\\aa\\y\\10.jpg", image_type::grayscale);
  vec_t vec10 = bgr_img10.to_vec();
  tiny_dnn::image<> bgr_img11("C:\\Users\\Jess\\Desktop\\aa\\y\\11.jpg", image_type::grayscale);
  vec_t vec11 = bgr_img11.to_vec();
  tiny_dnn::image<> bgr_img12("C:\\Users\\Jess\\Desktop\\aa\\y\\12.jpg", image_type::grayscale);
  vec_t vec12 = bgr_img12.to_vec();
  tiny_dnn::image<> bgr_img13("C:\\Users\\Jess\\Desktop\\aa\\y\\13.jpg", image_type::grayscale);
  vec_t vec13 = bgr_img13.to_vec();
  tiny_dnn::image<> bgr_img14("C:\\Users\\Jess\\Desktop\\aa\\y\\14.jpg", image_type::grayscale);
  vec_t vec14 = bgr_img14.to_vec();
  tiny_dnn::image<> bgr_img15("C:\\Users\\Jess\\Desktop\\aa\\y\\15.jpg", image_type::grayscale);
  vec_t vec15 = bgr_img15.to_vec();
  tiny_dnn::image<> bgr_img16("C:\\Users\\Jess\\Desktop\\aa\\y\\16.jpg", image_type::grayscale);
  vec_t vec16 = bgr_img16.to_vec();
  tiny_dnn::image<> bgr_img17("C:\\Users\\Jess\\Desktop\\aa\\y\\17.jpg", image_type::grayscale);
  vec_t vec17 = bgr_img17.to_vec();
  tiny_dnn::image<> bgr_img18("C:\\Users\\Jess\\Desktop\\aa\\y\\18.jpg", image_type::grayscale);
  vec_t vec18 = bgr_img18.to_vec();
  tiny_dnn::image<> bgr_img19("C:\\Users\\Jess\\Desktop\\aa\\y\\19.jpg", image_type::grayscale);
  vec_t vec19 = bgr_img19.to_vec();
  tiny_dnn::image<> bgr_img20("C:\\Users\\Jess\\Desktop\\aa\\y\\20.jpg", image_type::grayscale);
  vec_t vec20 = bgr_img20.to_vec();
  tiny_dnn::image<> bgr_img21("C:\\Users\\Jess\\Desktop\\aa\\y\\21.pgm", image_type::grayscale);
  vec_t vec21 = bgr_img21.to_vec();
  tiny_dnn::image<> bgr_img22("C:\\Users\\Jess\\Desktop\\aa\\y\\22.pgm", image_type::grayscale);
  vec_t vec22 = bgr_img22.to_vec();
  tiny_dnn::image<> bgr_img23("C:\\Users\\Jess\\Desktop\\aa\\y\\23.pgm", image_type::grayscale);
  vec_t vec23 = bgr_img23.to_vec();
  tiny_dnn::image<> bgr_img24("C:\\Users\\Jess\\Desktop\\aa\\y\\24.pgm", image_type::grayscale);
  vec_t vec24 = bgr_img24.to_vec();
  tiny_dnn::image<> bgr_img25("C:\\Users\\Jess\\Desktop\\aa\\y\\25.pgm", image_type::grayscale);
  vec_t vec25 = bgr_img25.to_vec();
  tiny_dnn::image<> bgr_img26("C:\\Users\\Jess\\Desktop\\aa\\y\\26.pgm", image_type::grayscale);
  vec_t vec26 = bgr_img26.to_vec();
  tiny_dnn::image<> bgr_img27("C:\\Users\\Jess\\Desktop\\aa\\y\\27.pgm", image_type::grayscale);
  vec_t vec27 = bgr_img27.to_vec();
  tiny_dnn::image<> bgr_img28("C:\\Users\\Jess\\Desktop\\aa\\y\\28.pgm", image_type::grayscale);
  vec_t vec28 = bgr_img28.to_vec();
  tiny_dnn::image<> bgr_img29("C:\\Users\\Jess\\Desktop\\aa\\y\\29.pgm", image_type::grayscale);
  vec_t vec29 = bgr_img29.to_vec();
  tiny_dnn::image<> bgr_img30("C:\\Users\\Jess\\Desktop\\aa\\y\\30.pgm", image_type::grayscale);
  vec_t vec30 = bgr_img30.to_vec();
  tiny_dnn::image<> bgr_img31("C:\\Users\\Jess\\Desktop\\aa\\y\\31.pgm", image_type::grayscale);
  vec_t vec31 = bgr_img31.to_vec();
  tiny_dnn::image<> bgr_img32("C:\\Users\\Jess\\Desktop\\aa\\y\\32.pgm", image_type::grayscale);
  vec_t vec32 = bgr_img32.to_vec();
  tiny_dnn::image<> bgr_img33("C:\\Users\\Jess\\Desktop\\aa\\y\\33.pgm", image_type::grayscale);
  vec_t vec33 = bgr_img33.to_vec();
  tiny_dnn::image<> bgr_img34("C:\\Users\\Jess\\Desktop\\aa\\y\\34.pgm", image_type::grayscale);
  vec_t vec34 = bgr_img34.to_vec();
  tiny_dnn::image<> bgr_img35("C:\\Users\\Jess\\Desktop\\aa\\y\\35.pgm", image_type::grayscale);
  vec_t vec35 = bgr_img35.to_vec();
  tiny_dnn::image<> bgr_img36("C:\\Users\\Jess\\Desktop\\aa\\y\\36.pgm", image_type::grayscale);
  vec_t vec36 = bgr_img36.to_vec();
  tiny_dnn::image<> bgr_img37("C:\\Users\\Jess\\Desktop\\aa\\y\\37.pgm", image_type::grayscale);
  vec_t vec37 = bgr_img37.to_vec();
  tiny_dnn::image<> bgr_img38("C:\\Users\\Jess\\Desktop\\aa\\y\\38.pgm", image_type::grayscale);
  vec_t vec38 = bgr_img38.to_vec();
  tiny_dnn::image<> bgr_img39("C:\\Users\\Jess\\Desktop\\aa\\y\\39.pgm", image_type::grayscale);
  vec_t vec39 = bgr_img39.to_vec();
  tiny_dnn::image<> bgr_img40("C:\\Users\\Jess\\Desktop\\aa\\y\\40.pgm", image_type::grayscale);
  vec_t vec40 = bgr_img40.to_vec();

  images.push_back(vec21);
  images.push_back(vec22);
  images.push_back(vec23);
  images.push_back(vec24);
  images.push_back(vec25);
  images.push_back(vec26);
  images.push_back(vec27);
  images.push_back(vec28);
  images.push_back(vec29);
  images.push_back(vec30);
  images.push_back(vec31);
  images.push_back(vec32);
  images.push_back(vec33);
  images.push_back(vec34);
  images.push_back(vec35);
  images.push_back(vec36);
  images.push_back(vec37);
  images.push_back(vec38);
  images.push_back(vec39);
  images.push_back(vec40);
  images.push_back(vec);
  images.push_back(vec2);
  images.push_back(vec3);
  images.push_back(vec4);
  images.push_back(vec5);
  images.push_back(vec6);
  images.push_back(vec7);
  images.push_back(vec8);
  images.push_back(vec9);
  images.push_back(vec10);
  images.push_back(vec11);
  images.push_back(vec12);
  images.push_back(vec13);
  images.push_back(vec14);
  images.push_back(vec15);
  images.push_back(vec16);
  images.push_back(vec17);
  images.push_back(vec18);
  images.push_back(vec19);
  images.push_back(vec20);*/
  int n = 463+213;
  int y = 213;
  int i = 0;
  for(i; i < y; i++){
    tiny_dnn::image<> bgr_img(argv[i+1], image_type::grayscale);
    vec_t vec = bgr_img.to_vec();
    label.push_back(1);
    images.push_back(vec);
  }
  cout << "Imagens lidas: " << images.size()<<endl;
  for(i; i < n; i++){
    tiny_dnn::image<> bgr_img(argv[i+1], image_type::grayscale);
    vec_t vec = bgr_img.to_vec();
    label.push_back(0);
    images.push_back(vec);
  }
  cout << "Imagens lidas: " << images.size()<<endl;
  normalizar(images);
//=======================SETUP A REDE NEURAL===========================

  net << conv(32,64,5,1,20) << relu()
      << max_pool(28,60,20,2) << relu()
      << conv(14, 30, 5,20,40) << relu()
      << max_pool(10,26,40,2) << relu()
      << fc(5*13*40, 200) << relu() 
      << fc(200, 2) << sigmoid();

  int n_train_epochs=50;
  int n_minibatch=10;

  adagrad optimizer;
  cout << "start training" << endl;
  progress_display disp(images.size());
  timer t;
  
  int epoch = 1;
 
  int omelhor=-1;
  auto on_enumerate_epoch = [&]() {
     cout << "Epoch " << epoch << "/" << n_train_epochs << " finished. "
     << t.elapsed() << "s elapsed." << endl;
     ++epoch;
     result res = net.test(images, label);
     cout << res.num_success << "/" << res.num_total << endl;

      if (omelhor<res.num_success) {
        omelhor=res.num_success;
        string nomearq="cnn_noact2.net";
        net.save(nomearq);
        cout << "Gravado arquivo " << nomearq << endl;
      }
      disp.restart(images.size());
      t.restart();
  };
  auto on_enumerate_minibatch = [&]() { disp += n_minibatch; };
  net.train<mse>(optimizer, images, label, n_minibatch,
  n_train_epochs, on_enumerate_minibatch, on_enumerate_epoch);
  cout << "end training." << endl;

  cout << "[";
  for (unsigned i=0; i<images.size(); i++) {
    vec_t result=net.predict(images[i]);
    cout << argMax(result) << " ";
  }
cout << "\b]\n"<<endl;
cout << "antes"<<endl;



  VideoCapture capture("c:\\Users\\Jess\\Desktop\\videos\\aaa.avi");
  cout << "depois"<<endl;
  if (!capture.isOpened())
    throw "Error when reading steam_avi";
  
  
  namedWindow("teste", 1);
  int cont=0;
  while(true){
      Mat_<COR> frame;
    
      capture >> frame;
      
      if (frame.empty())
      {
        printf(" --(!) No captured frame -- Break!");
        break;
      }
      
      Mat_<GRY> fotoGray;
      cvtColor(frame,fotoGray,CV_BGR2GRAY);
      cv::resize(fotoGray, fotoGray, cv::Size(32,64));
      //normalizar(fotoGray);
      vec_t result = net.predict(fotoGray);
      if (argMax(result)==1)
      {
        cout << argMax(result) << " ";
      }
      imshow("teste", frame);
      char c = (char)waitKey(30);
      if (c == 27) { break; } // escape
  }
  return 0;
}
