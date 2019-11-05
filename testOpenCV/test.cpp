#include <stdio.h>
#include "Converter.h"
#include "EdgeDetector.h"
#include "Blur.h"
#include "ColorTransformer.h"
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"

using namespace cv;

int main()
{
	Mat image = imread("X:\\image004.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat desImage1, desImage2;
	Blur bl;
	EdgeDetector edge;

	if (edge.DetectEdge(image, desImage1, 3, 3, 4) == 0) {
		printf("Khong load duoc anh!!\n");
		system("pause");
	}
	else {
		namedWindow("Anh goc");
		imshow("Anh goc", image);
		namedWindow("Tu viet");
		imshow("Tu viet", desImage1);
		waitKey(0);
	}
	system("pause");
	return 0;
}