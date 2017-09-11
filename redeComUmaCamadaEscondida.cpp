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

static double * norm_max;

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

  net << fc(32*64, 100) << relu() 
      << fc(100, 2) << sigmoid();

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
