#include "Converter.h"
#include <math.h>

int maximum(int num1, int num2, int num3) {
	if (num1 >= num2) {
		if (num1 >= num3) return num1;
		else return num3;
	}
	else {
		if (num2 >= num3) return num2;
		else return num3;
	}
}
int minimize(int num1, int num2, int num3) {
	if (num1 <= num2) {
		if (num1 <= num3) return num1;
		else return num3;
	}
	else {
		if (num2 <= num3) return num2;
		else return num3;
	}
}
void RGBtoHSV(int r, int g, int b, float &h, float &s, float &v)
{
	int max = maximum(r, g, b);
	int min = minimize(r, g, b);
	int delta = max - min;
	v = max;
	if (v == 0 || delta == 0) s = 0;
	else s = delta / v;
	if (delta == 0) h = 0;
	else if (v == r) h = 60 * (g - b) / delta;
	else if (v == g) h = 120 + 60 * (b - r) / delta;
	else if (v == b) h = 240 + 60 * (r - g) / delta;
}
void HSVtoRGB(int h, int s, int v, int &r, int &g, int &b)
{
	float C = v*s*1.0 / 255;	//chu y cho nay
	float H_tmp = h / 30;			//chu y chi cho 60 hay 30
	float X = C*(1 - abs(int(H_tmp) % 2 - 1));
	int red = 0, green = 0, blue = 0;
	if (H_tmp >= 0 && H_tmp < 1) {
		red = C;
		green = X;
	}
	else if (H_tmp >= 1 && H_tmp < 2) {
		red = X;
		green = C;
	}
	else if (H_tmp >= 2 && H_tmp < 3) {
		green = C;
		blue = X;
	}
	else if (H_tmp >= 3 && H_tmp < 4) {
		green = X;
		blue = C;
	}
	else if (H_tmp >= 4 && H_tmp < 5) {
		red = X;
		blue = C;
	}
	else if (H_tmp >= 5 && H_tmp < 6) {
		red = C;
		blue = X;
	}
	float m = v - C;
	r = round(red + m);
	g = round(green + m);
	b = round(blue + m);
}

int Converter::RGB2GrayScale(const Mat & sourceImage, Mat & destinationImage)
{
	if (sourceImage.data == NULL) return 0;

	int width = sourceImage.cols, height = sourceImage.rows;
	destinationImage = Mat(height, width, CV_8UC1);
	for (int x = 0; x < height;x++) {
		for (int y = 0; y < width; y++) {
			destinationImage.at<uchar>(x, y) = (sourceImage.at<cv::Vec3b>(x, y)[2] * 2 + sourceImage.at<cv::Vec3b>(x, y)[1] * 5 + sourceImage.at<cv::Vec3b>(x, y)[0]) / 8;;
		}
	}
	return 1;
}

int Converter::GrayScale2RGB(const Mat & sourceImage, Mat & destinationImage)
{
	if (sourceImage.data == NULL) return 0;

	int width = sourceImage.cols, height = sourceImage.rows;
	destinationImage = Mat(height, width, CV_8UC3);
	for (int x = 0; x<height; x++)
		for (int y = 0; y<width; y++)
			if (sourceImage.at<uchar>(x, y) >= 128) {
				destinationImage.at<cv::Vec3b>(x, y)[0] = 255;
				destinationImage.at<cv::Vec3b>(x, y)[1] = 255;
				destinationImage.at<cv::Vec3b>(x, y)[2] = 255;
			}
			else {
				destinationImage.at<cv::Vec3b>(x, y)[0] = 0;
				destinationImage.at<cv::Vec3b>(x, y)[1] = 0;
				destinationImage.at<cv::Vec3b>(x, y)[2] = 0;
			}
	return 1;
}

int Converter::RGB2HSV(const Mat & sourceImage, Mat & destinationImage)
{
	if (sourceImage.data == NULL) return 0;

	int width = sourceImage.cols, height = sourceImage.rows;
	destinationImage = Mat(height, width, CV_8UC3);

	for (int x = 0; x < height; x++)
		for (int y = 0; y < width; y++) {
			int r = sourceImage.at<cv::Vec3b>(x, y)[2];
			int g = sourceImage.at<cv::Vec3b>(x, y)[1];
			int b = sourceImage.at<cv::Vec3b>(x, y)[0];

			float h, s, v;
			RGBtoHSV(r, g, b, h, s, v);

			destinationImage.at<cv::Vec3b>(x, y)[0] = round(h / 2);
			destinationImage.at<cv::Vec3b>(x, y)[1] = round(s * 255);
			destinationImage.at<cv::Vec3b>(x, y)[2] = round(v);
		}
	return 1;
}

int Converter::HSV2RGB(const Mat & sourceImage, Mat & destinationImage)
{
	if (sourceImage.data == NULL) return 0;

	int width = sourceImage.cols, height = sourceImage.rows;
	destinationImage = Mat(height, width, CV_8UC3);

	for (int x = 0; x < height; x++)
		for (int y = 0; y < width; y++) {
			int v = sourceImage.at<cv::Vec3b>(x, y)[2];
			int s = sourceImage.at<cv::Vec3b>(x, y)[1];
			int h = sourceImage.at<cv::Vec3b>(x, y)[0];

			int r, g, b;

			HSVtoRGB(h, s, v, r, g, b);

			destinationImage.at<cv::Vec3b>(x, y)[0] = saturate_cast<uchar>(b);
			destinationImage.at<cv::Vec3b>(x, y)[1] = saturate_cast<uchar>(g);
			destinationImage.at<cv::Vec3b>(x, y)[2] = saturate_cast<uchar>(r);
		}
	return 1;
}

int Converter::Convert(Mat & sourceImage, Mat & destinationImage, int type)
{
	if (sourceImage.data == NULL || type > 3 || type < 0) return 0;
	switch (type)
	{
	case 0: RGB2GrayScale(sourceImage, destinationImage); break;
	case 1: GrayScale2RGB(sourceImage, destinationImage); break;
	case 2: RGB2HSV(sourceImage, destinationImage); break;
	case 3: HSV2RGB(sourceImage, destinationImage); break;
	default:
		break;
	}
	return 1;
}

int Converter::Convert(IplImage * sourceImage, IplImage * destinationImage, int type)
{
	return 0;
}

Converter::Converter()
{
}

Converter::~Converter()
{
}
