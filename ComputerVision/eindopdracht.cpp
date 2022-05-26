#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Project 3 - License Plate Detector //////////////////////

void preProcessPlate(Mat& img, Mat& resultImg, int i = 0);
void getContours(Mat imgDil, Mat& img);

int cannyThreshold = 25;

void licensePlate(const Mat& inputImg, Mat& outputImg);
void getCharacters(const Mat& inputImg, Mat& outputImg);
void preProcces1(const Mat& inputImg, Mat& outputImg);


void main()
{
	Mat img = imread("Resources/KentekensRU.jpg");

	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");

	if (plateCascade.empty()) { cout << "XML file not loaded" << endl; }

	vector<Rect> plates;

	// while (true)
	// {
	plateCascade.detectMultiScale(img, plates, 1.1, 10);

	for (int i = 0; i < plates.size(); i++)
	{
		Mat plateImgOriginal = img(plates[i]);
		// Mat plateImgResult;
		// preProcessPlate(plateImgOriginal, plateImgResult, i);
		// getContours(plateImgResult, plateImgOriginal);


		licensePlate(plateImgOriginal, plateImgOriginal);

		imshow(to_string(i), plateImgOriginal);
		imwrite("/Resources/Plates/" + to_string(i) + ".png", plateImgOriginal);
		//rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);
	}

	imshow("Image", img);
	waitKey(0);
	// }
}

void licensePlate(const Mat& inputImg, Mat& outputImg)
{
	Mat  zoomImg, imgDil,test;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;


	preProcces1(inputImg, imgDil);

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	//drawContours(outputImg, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());


	for (int i = 0; i < contours.size(); i++)
	{
		int area = (int)contourArea(contours[i]);
		cout << area << endl;

		if (area < 10000) continue;

		cout << "ran" << endl;

		float peri = arcLength(contours[i], true);
		approxPolyDP(contours[i], conPoly[i], 0.01 * peri, true);
		cout << conPoly[i].size() << endl;
		boundRect[i] = boundingRect(conPoly[i]);

		zoomImg = inputImg(boundRect[i]);

		getCharacters(zoomImg, test);

		outputImg = test;
		
		//outputImg = inputImg({ boundRect[i].tl() ,boundRect[i].br() });

		//rectangle(outputImg, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
	}

	//getCharacters(zoomImg, outputImg);

	//Canny(imgBlur, imgCanny, cannyThreshold, cannyThreshold * 3);

	//outputImg = imgBlur;
}

void getCharacters(const Mat& inputImg, Mat& outputImg)
{

	preProcces1(inputImg, outputImg);



}



void preProcces1(const Mat& inputImg, Mat& outputImg)
{
	Mat imgGray, imgBlur, imgCanny;
	cvtColor(inputImg, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(0, 0), 3, 3);
	Canny(imgBlur, imgCanny, 20, 60);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

	dilate(imgCanny, outputImg, kernel);
	// erode(imgCanny, outputImg, kernel);
}

//
// void getContours(Mat imgDil, Mat& img)
// {
// 	vector<vector<Point>> contours;
// 	vector<Vec4i> hierarchy;
//
// 	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
// 	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);
//
// 	vector<vector<Point>> conPoly(contours.size());
// 	vector<Rect> boundRect(contours.size());
//
// 	for (int i = 0; i < contours.size(); i++)
// 	{
// 		int area = contourArea(contours[i]);
// 		cout << area << endl;
// 		string objectType;
//
// 		if (area > 10000)
// 		{
// 			float peri = arcLength(contours[i], true);
// 			approxPolyDP(contours[i], conPoly[i], 0.03 * peri, true);
// 			cout << conPoly[i].size() << endl;
// 			boundRect[i] = boundingRect(conPoly[i]);
//
// 			int objCor = (int)conPoly[i].size();
//
// 			if (objCor != 4) continue;
//
// 			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
// 			//rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
// 			//putText(img, objectType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0, 69, 255), 2);
// 		}
// 	}
// }
//
// void preProcessPlate(Mat& img, Mat& resultImg, int i)
// {
// 	Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;
// 	cvtColor(img, imgGray, COLOR_BGR2GRAY);
// 	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
// 	Canny(imgBlur, imgCanny, cannyThreshold, cannyThreshold * 3);
// 	//imshow(to_string(i+10), imgCanny);
// 	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
// 	dilate(imgCanny, resultImg, kernel);
// }
