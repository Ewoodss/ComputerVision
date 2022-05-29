#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Project 3 - License Plate Detector //////////////////////

void preProcessPlate(Mat& img, Mat& resultImg, int i = 0);
void getContours(Mat imgDil, Mat& img);
void deleteDirectoryContents(const std::string& dir_path);

int cannyThreshold = 25;
void licensePlate(const Mat& inputImg, Mat& outputImg, int plateNumber);
void getCharacters(const Mat& inputImg, Mat& outputImg, int plateNumber);
void preProcces1(const Mat& inputImg, Mat& outputImg);


void main()
{
	Mat img = imread("Resources/KentekensRU.jpg");

	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");

	if (plateCascade.empty()) { cout << "XML file not loaded" << endl; }

	vector<Rect> plates;

	plateCascade.detectMultiScale(img, plates, 1.1, 10);

	for (int i = 0; i < plates.size(); i++)
	{
		Mat plateImgOriginal = img(plates[i]);
		licensePlate(plateImgOriginal, plateImgOriginal, i);
		imshow(to_string(i), plateImgOriginal);
	}

	imshow("Image", img);
	waitKey(0);
}

void licensePlate(const Mat& inputImg, Mat& outputImg, int plateNumber)
{
	Mat zoomImg, imgDil, test;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;


	preProcces1(inputImg, imgDil);
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<Point> conPoly;
	Rect boundRect;


	vector<Point> biggestContour;
	double biggestArea = -1;

	for (auto contour : contours)
	{
		auto erea = contourArea(contour);
		if (erea > biggestArea)
		{
			biggestArea = erea;
			biggestContour = contour;
		}
	}

	float peri = arcLength(biggestContour, true);
	approxPolyDP(biggestContour, conPoly, 0.01 * peri, true);
	cout << conPoly.size() << endl;
	boundRect = boundingRect(conPoly);

	try
	{
		rectangle(inputImg, boundRect.tl(), boundRect.br(), Scalar(0, 255, 0), 5);
		zoomImg = inputImg(boundRect);

		getCharacters(zoomImg, test, plateNumber);
	}
	catch (Exception e)
	{
		cout << e.what() << endl;
	}

	outputImg = test;
}

void getCharacters(const Mat& inputImg, Mat& outputImg, int plateNumber)
{
	inputImg.copyTo(outputImg);

	Mat preProcessOutput;
	preProcces1(inputImg, preProcessOutput);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(preProcessOutput, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		float peri = arcLength(contours[i], true);
		approxPolyDP(contours[i], conPoly[i], 0.01 * peri, true);
		boundRect[i] = boundingRect(conPoly[i]);
		int area = (boundRect[i].height) * boundRect[i].width;
		float aspectRatio = (float)boundRect[i].height / boundRect[i].width;

		if (area < 400) continue;
		if (aspectRatio < 1 || aspectRatio > 3) continue;

		rectangle(outputImg, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 1);
		Mat resultImg = inputImg(boundRect[i]);
		std::string imageName = "nummerbord" + to_string(plateNumber) + "_ " + to_string(i);

		imwrite("./Resources/Plates/" + imageName + ".jpg", resultImg);
	}
}


void preProcces1(const Mat& inputImg, Mat& outputImg)
{
	Mat imgGray, imgBlur, imgCanny, imgDilate;
	cvtColor(inputImg, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(0, 0), 3, 3);

	auto canny = 20; //needs to be changed per image

	Canny(imgBlur, imgCanny, canny, canny * 3);
	Mat dilateKernel = getStructuringElement(MORPH_DILATE, Size(4, 3));
	Mat erodeKernel = getStructuringElement(MORPH_ERODE, Size(3, 3));

	dilate(imgCanny, outputImg, dilateKernel);
}
