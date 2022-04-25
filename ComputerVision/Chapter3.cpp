#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Resize and Crop //////////////////////

void main() {

	//string path = "Resources/test.png";
	VideoCapture cap(0);
	Mat img;
	Mat imgResize, imgCrop;

	cap.read(img);

	//cout << img.size() << endl;
	resize(img, imgResize, Size(), 0.5, 0.5);

	Rect roi(200, 100, 300, 300);
	imgCrop = img(roi);

	imshow("Image", img);
	imshow("Image Resize", imgResize);
	imshow("Image Crop", imgCrop);
	waitKey(0);
}