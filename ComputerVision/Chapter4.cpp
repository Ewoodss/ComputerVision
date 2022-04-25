#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//////////////  Draw Shapes and Text //////////////////////

void main()
{
	// Blank Image 
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));

	circle(img, Point(128, 128), 120, Scalar(0, 69, 255), 8);
	circle(img, Point(384, 128), 120, Scalar(0, 69, 255), 8);


	rectangle(img, Point(128, 384 + 64), Point(128 * 3, 256 + 64), Scalar(0, 69, 255), LINE_AA);

	line(img, Point(128, 384), Point(128 * 3, 384), Scalar(0, 69, 255), 2);

	putText(img, "~~~~Snor~~~~", Point(160, 280), FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 69, 255), 2);

	imshow("Image", img);
	waitKey(0);
}
