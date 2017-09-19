#include <cekeikon.h>
#include <iostream>
 
using namespace cv;
using namespace std;
 
int main(int argc, char** argv)
{
    int n = argc-1;
    for(int i = 0; i < n; i++){
        Mat src = imread(argv[i], CV_LOAD_IMAGE_UNCHANGED);
        double angle = -90;
        if(src.cols > src.rows){
            // get rotation matrix for rotating the image around its center
            Point2f center(src.cols/2.0, src.rows/2.0);
            Mat rot = getRotationMatrix2D(center, angle, 1.0);
            // determine bounding rectangle
            Rect bbox = RotatedRect(center,src.size(), angle).boundingRect();
            // adjust transformation matrix
            rot.at<double>(0,2) += bbox.width/2.0 - center.x;
            rot.at<double>(1,2) += bbox.height/2.0 - center.y;

            Mat dst;
            warpAffine(src, dst, rot, bbox.size());
            imwrite(argv[i], dst);
            
        }
    }
    return 0;
}