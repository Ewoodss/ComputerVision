#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Warp Images  //////////////////////

void main() {

	string path = "Resources/TheFutureOfEnergy3.jpg";
	Mat img = imread(path);

	Mat matrix, imgWarp;
	float w = 210*3, h = 297*3;

	Point2f src[4] = { {1665, 159},{3022, 177},{1465, 2118},{3311, 2104} };
	Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	for (int i = 0; i < 4; i++)
	{
		circle(img, src[i], 10, Scalar(0, 0, 255), FILLED);
	}

	resize(img, img, Size(), 0.4, 0.4);

	imshow("Image", img);
	imshow("Image Warp", imgWarp);
	waitKey(0);
}

