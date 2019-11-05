#include "ColorTransformer.h"

unsigned long maximum(unsigned long H[]) {
	unsigned long max = H[0];
	for (int i = 0; i < max_Histogram; i++)
		if (max < H[i]) max = H[i];
	return max;
}

int ColorTransformer::ChangeBrightness(const Mat & sourceImage, Mat & destinationImage, uchar b)
{
	if (sourceImage.data == NULL) return 0;

	int width = sourceImage.cols, height = sourceImage.rows;
	int nchannel = sourceImage.channels();
	destinationImage = Mat(height, width, CV_AA);
	for (int x = 0;x<height;x++)
		for (int y = 0; y < width; y++) {
			for (int i = 0; i < nchannel; i++)
				destinationImage.at<cv::Vec3b>(x, y)[i] = saturate_cast<uchar>(sourceImage.at<cv::Vec3b>(x, y)[i] + b);
		}

	return 1;
}

int ColorTransformer::ChangeContrast(const Mat & sourceImage, Mat & destinationImage, float c)
{
	if (sourceImage.data == NULL) return 0;

	int width = sourceImage.cols, height = sourceImage.rows;
	int nchannel = sourceImage.channels();
	destinationImage = Mat(height, width, CV_AA);
	for (int x = 0; x<height; x++)
		for (int y = 0; y < width; y++) {
			for (int i = 0; i < nchannel; i++)
				destinationImage.at<cv::Vec3b>(x, y)[i] = saturate_cast<uchar>(c*sourceImage.at<cv::Vec3b>(x, y)[i]);
		}

	return 1;
}

int ColorTransformer::HistogramEqualization(const Mat & sourceImage, Mat & destinationImage)
{
	if (sourceImage.data == NULL) return 0;

	int height = sourceImage.rows, width = sourceImage.cols;
	int nchannel = sourceImage.channels();

	if (nchannel == 1) {
		unsigned long H[max_Histogram];
		unsigned long T[max_Histogram];
		memset(H, 0, max_Histogram);
		memset(T, 0, max_Histogram);

		for (int x = 0; x < height; x++)
			for (int y = 0; y < width; y++) {
				int index = sourceImage.at<uchar>(x, y);
				H[index] += 1;
			}

		T[0] = H[0];
		for (int i = 1; i < max_Histogram; i++)
			T[i] = T[i - 1] + H[i];
		for (int i = 0; i < max_Histogram; i++)
			T[i] = round(T[i] * (max_Histogram - 1) / (width*height));

		destinationImage = Mat(height, width, CV_8UC1);
		for (int x = 0; x < height; x++)
			for (int y = 0; y < width; y++)
				destinationImage.at<uchar>(x, y) = T[sourceImage.at<uchar>(x, y)];
	}
	if (nchannel == 3) {
		unsigned long rH[max_Histogram], gH[max_Histogram], bH[max_Histogram];
		unsigned long rT[max_Histogram], gT[max_Histogram], bT[max_Histogram];
		memset(rH, 0, max_Histogram);
		memset(gH, 0, max_Histogram);
		memset(bH, 0, max_Histogram);
		memset(rT, 0, max_Histogram);
		memset(gT, 0, max_Histogram);
		memset(bT, 0, max_Histogram);

		for (int x= 0;x<height;x++)
			for (int y = 0; y < width; y++) {
				rH[sourceImage.at<cv::Vec3b>(x, y)[2]] += 1;
				gH[sourceImage.at<cv::Vec3b>(x, y)[1]] += 1;
				bH[sourceImage.at<cv::Vec3b>(x, y)[0]] += 1;
			}

		rT[0] = rH[0];
		gT[0] = gT[0];
		bT[0] = bH[0];
		for (int i = 1; i < max_Histogram; i++) {
			rT[i] = rT[i - 1] + rH[i];
			gT[i] = gT[i - 1] + gH[i];
			bT[i] = bT[i - 1] + bH[i];
		}
		for (int i = 0; i < max_Histogram; i++) {
			rT[i] = round(rT[i] * (max_Histogram - 1) / (width*height));
			gT[i] = round(gT[i] * (max_Histogram - 1) / (width*height));
			bT[i] = round(bT[i] * (max_Histogram - 1) / (width*height));
		}

		destinationImage = Mat(height, width, CV_8UC3);
		for (int x = 0; x < height; x++)
			for (int y = 0; y < width; y++) {
				destinationImage.at<cv::Vec3b>(x, y)[0] = bT[sourceImage.at<cv::Vec3b>(x, y)[0]];
				destinationImage.at<cv::Vec3b>(x, y)[1] = gT[sourceImage.at<cv::Vec3b>(x, y)[1]];
				destinationImage.at<cv::Vec3b>(x, y)[2] = rT[sourceImage.at<cv::Vec3b>(x, y)[2]];
			}
	}

	return 1;
}

int ColorTransformer::CalcHistogram(const Mat & sourceImage, Mat & histogram)
{
	if (sourceImage.data == NULL) return 0;

	int width = sourceImage.cols, height = sourceImage.rows;
	int nchannel = sourceImage.channels();
	if (nchannel == 1) {
		histogram = Mat(1, 256, CV_AA);
		for (int i = 0; i < 256; i++)
			histogram.at<uchar>(0, i) = 0;
		for (int x = 0; x < height; x++)
			for (int y = 0; y < width; y++) {
				unsigned int index = sourceImage.at<uchar>(x, y);
				histogram.at<uchar>(0, index) += 1;
			}
	}
	if (nchannel == 3) {
		histogram = Mat(1, 256, CV_AA);
		for (int i = 0; i < 256; i++) {
			histogram.at<cv::Vec3b>(0, i)[0] = 0;
			histogram.at<cv::Vec3b>(0, i)[1] = 0;
			histogram.at<cv::Vec3b>(0, i)[2] = 0;
		}
		for (int x = 0; x < height; x++)
			for (int y = 0; y < width; y++) {
				int rIndex = sourceImage.at<cv::Vec3b>(x, y)[2];
				int gIndex = sourceImage.at<cv::Vec3b>(x, y)[1];
				int bIndex = sourceImage.at<cv::Vec3b>(x, y)[0];

				histogram.at<cv::Vec3b>(0, bIndex)[0] += 1;
				histogram.at<cv::Vec3b>(0, gIndex)[1] += 1;
				histogram.at<cv::Vec3b>(0, rIndex)[2] += 1;
			}
	}

	return 1;
}

int ColorTransformer::DrawHistogram(const Mat & sourceImage, Mat & histImage)
{
	if (sourceImage.data == NULL) return 0;

	int height = sourceImage.rows, width = sourceImage.cols;
	int nchannel = sourceImage.channels();

	if (nchannel == 1) {
		unsigned long H[max_Histogram];
		memset(H, 0, max_Histogram);

		for (int x = 0; x < height; x++)
			for (int y = 0; y < width; y++)
				H[sourceImage.at<uchar>(x, y)] += 1;

		histImage = Mat(width*height, max_Histogram, CV_8UC1);
		for (int x = 0; x < max_Histogram; x++)
			for (int y = 0; y < width*height; y++) {
				if (y <= H[x]) histImage.at<uchar>(y, x) = 255;
				else histImage.at<uchar>(y, x) = 0;
			}
	}
	if (nchannel == 3) {
		unsigned long rH[max_Histogram], gH[max_Histogram], bH[max_Histogram];
		memset(rH, 0, max_Histogram);
		memset(gH, 0, max_Histogram);
		memset(bH, 0, max_Histogram);

		for (int x = 0; x < height; x++)
			for (int y = 0; y < width; y++) {
				rH[sourceImage.at<cv::Vec3b>(x, y)[2]] += 1;
				gH[sourceImage.at<cv::Vec3b>(x, y)[1]] += 1;
				bH[sourceImage.at<cv::Vec3b>(x, y)[0]] += 1;
			}
		histImage = Mat(width*height, max_Histogram * 3, CV_8UC3);
		for (int x = 0; x < max_Histogram; x++)
			for (int y = 0; y < width*height; y++) {
				if (y <= rH[x]) {
					histImage.at<cv::Vec3b>(y, x)[2] = 255;
					histImage.at<cv::Vec3b>(y, x)[1] = 0;
					histImage.at<cv::Vec3b>(y, x)[0] = 0;
				}
				else {
					histImage.at<cv::Vec3b>(y, x)[2] = 255;
					histImage.at<cv::Vec3b>(y, x)[1] = 255;
					histImage.at<cv::Vec3b>(y, x)[0] = 255;
				}
				if (y <= gH[x]) {
					histImage.at<cv::Vec3b>(y, x + 256)[2] = 0;
					histImage.at<cv::Vec3b>(y, x + 256)[1] = 255;
					histImage.at<cv::Vec3b>(y, x + 256)[0] = 0;
				}
				else {
					histImage.at<cv::Vec3b>(y, x + 256)[2] = 255;
					histImage.at<cv::Vec3b>(y, x + 256)[1] = 255;
					histImage.at<cv::Vec3b>(y, x + 256)[0] = 255;
				}
				if (y <= bH[x]) {
					histImage.at<cv::Vec3b>(y, x + 512)[2] = 0;
					histImage.at<cv::Vec3b>(y, x + 512)[1] = 0;
					histImage.at<cv::Vec3b>(y, x + 512)[0] = 255;
				}
				else {
					histImage.at<cv::Vec3b>(y, x + 512)[2] = 255;
					histImage.at<cv::Vec3b>(y, x + 512)[1] = 255;
					histImage.at<cv::Vec3b>(y, x + 512)[0] = 255;
				}
			}
	}
	return 1;
}

ColorTransformer::ColorTransformer()
{
}

ColorTransformer::~ColorTransformer()
{
}
