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

	// while (true)
	// {
	plateCascade.detectMultiScale(img, plates, 1.1, 10);

	for (int i = 0; i < plates.size(); i++)
	{
		Mat plateImgOriginal = img(plates[i]);
		// Mat plateImgResult;
		// preProcessPlate(plateImgOriginal, plateImgResult, i);
		// getContours(plateImgResult, plateImgOriginal);

		licensePlate(plateImgOriginal, plateImgOriginal, i);

		imshow(to_string(i), plateImgOriginal);
		//rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);
	}

	imshow("Image", img);
	waitKey(0);
	// }
}

void licensePlate(const Mat& inputImg, Mat& outputImg, int plateNumber)
{
	Mat zoomImg, imgDil, test;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;


	preProcces1(inputImg, imgDil);

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	//drawContours(outputImg, contours, -1, Scalar(255, 0, 255), 2);

	vector<Point> conPoly;
	Rect boundRect;


	vector<Point> biggestContour;
	double biggestArea = -1;

	for (auto contour : contours)
	{
		auto erea = contourArea(contour);
		if (true)
		{
			if (erea > biggestArea)
			{
				biggestArea = erea;
				biggestContour = contour;
			}
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

	//outputImg = inputImg({ boundRect[i].tl() ,boundRect[i].br() });

	//rectangle(outputImg, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);


	//getCharacters(zoomImg, outputImg);

	//Canny(imgBlur, imgCanny, cannyThreshold, cannyThreshold * 3);

	//outputImg = imgBlur;
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
		//putText(outputImg, to_string(aspectRatio), {boundRect[i].x,boundRect[i].y}, FONT_HERSHEY_PLAIN, 1, Scalar(0, (i * 30 % 255), 255), 2);
	}
}


void preProcces1(const Mat& inputImg, Mat& outputImg)
{
	Mat imgGray, imgBlur, imgCanny,imgDilate;
	cvtColor(inputImg, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(0, 0), 3, 3);
	Canny(imgBlur, imgCanny, 20, 60);
	Mat dilateKernel = getStructuringElement(MORPH_RECT, Size(3, 3));

	dilate(imgCanny, outputImg, dilateKernel);
	imshow("testing", outputImg);
	// erode(imgCanny, outputImg, kernel);
}

void deleteDirectoryContents(const std::string& dir_path)
{
	//for (const auto& entry : std::filesystem::directory_iterator(dir_path))
	//	std::filesystem::remove_all(entry.path());
}
