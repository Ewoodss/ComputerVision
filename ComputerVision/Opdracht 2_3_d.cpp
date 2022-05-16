#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Basic Functions  //////////////////////

void main() {

	string path = "Resources/ballstripes.bmp";
	Mat img = imread(path);
	Mat imgResult;

	Mat kernel = getStructuringElement(MORPH_RECT, Size(4, 4));

	dilate(img, imgResult, kernel);
	erode(imgResult, imgResult, kernel);

	imshow("Image", img);
	imshow("Image Result", imgResult);
	waitKey(0);
}