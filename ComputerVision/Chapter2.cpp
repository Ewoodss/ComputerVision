#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

Mat src, src_gray;
Mat dst, detected_edges;
int lowThreshold = 0;
int maxThreshold = 0;
const char* window_name = "window";

int main()
{
	std::string path = "Resources/flower.jpg";
	src = imread(path);

	dst.create(src.size(), src.type());
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	namedWindow(window_name, WINDOW_AUTOSIZE);

	createTrackbar("Min Threshold:", window_name, &lowThreshold, 1000);
	createTrackbar("Max Threshold:", window_name, &maxThreshold, 1000);

	while (true)
	{
		blur(src_gray, detected_edges, Size(3, 3));
		Canny(detected_edges, detected_edges, lowThreshold, maxThreshold, 3);
		dst = Scalar::all(0);
		src.copyTo(dst, detected_edges);
		imshow(window_name, dst);

		waitKey(1000);
	}

	waitKey(0);
	return 0;
}
