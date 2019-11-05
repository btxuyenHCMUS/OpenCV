#pragma once
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <math.h>

#define		sigma	3		//define value sigma for gaussian of blur
#define		maxArrTmp	50	//define max number of size array temp

using namespace cv;

unsigned char getFxy(const Mat& sourceImage, int x, int y, int channel) {
	if (x < 0 || y < 0) return 0;
	if (x >= sourceImage.rows || y >= sourceImage.cols) return 0;

	if (sourceImage.channels() == 1) return sourceImage.at<uchar>(x, y);
	return sourceImage.at<cv::Vec3b>(x, y)[channel];
}

class Blur
{
public:
	/*
	Hàm làm trơn ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel
	0: Trung bình
	1: Trung vị
	2: Gaussian
	Hàm trả về
	0: nếu detect thành công
	1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method) {
		if (sourceImage.data == NULL) return 0;
		if (kHeight % 2 == 0 || kWidth % 2 == 0) return 0;
		int width = sourceImage.cols, height = sourceImage.rows;
		int nchannel = sourceImage.channels();

		//Nếu ảnh đưa vào là ảnh xám
		if (nchannel == 1) {
			destinationImage = Mat(height, width, CV_8UC1);
			//Phương pháp lọc trung bình
			if (method == 0) {
				for (int x = 0; x < height; x++)
					for (int y = 0; y < width; y++) {
						unsigned int value = 0;
						//Tính tích chập giữa source với kernel trung bình
						for (int i = -kHeight / 2; i <= kHeight / 2; i++)
							for (int j = -kWidth / 2; j <= kWidth / 2; j++) {
								value = value + getFxy(sourceImage, x - i, y - j, NULL);
							}
						value = value / (kWidth*kHeight);
						destinationImage.at<uchar>(x, y) = saturate_cast<uchar>(value);
					}
			}
			//Phương pháp lọc trung vị
			else if (method == 1) {
				for (int x = 0; x < height; x++)
					for (int y = 0; y < width; y++) {
						int index_Arr[maxArrTmp];
						int count = 0;
						//Tính tích chập và lưu vào mảng index_Arr
						for (int i = -kHeight / 2; i <= kHeight / 2; i++)
							for (int j = -kWidth / 2; j <= kWidth / 2; j++) {
								index_Arr[count] = getFxy(sourceImage, x - i, y - j, NULL);
								count++;
							}
						//Sắp xếp mảng tích chập
						for (int i = 0; i < count; i++)
							for (int j = i + 1; j < count; j++)
								if (index_Arr[i] > index_Arr[j]) {
									int tmp = index_Arr[i];
									index_Arr[i] = index_Arr[j];
									index_Arr[j] = tmp;
								}
						//Lấy ra giá trị Med(f(x, y))
						destinationImage.at<uchar>(x, y) = index_Arr[count / 2];
					}
			}
			//Phươnh pháp lọc gaussion
			else if (method == 2) {
				for (int x = 0; x < height; x++)
					for (int y = 0; y < width; y++) {
						unsigned int value = 0;
						//Tính tích chập bằng toán tử gaussion
						for (int i = -kHeight / 2; i <= kHeight / 2; i++)
							for (int j = -kWidth / 2; j <= kWidth / 2; j++) {
								value = value + getFxy(sourceImage, x - i, y - j, NULL) * 1 / (sqrt(2 * 3.14)*sigma)*exp(-(i*i + j*j) / (2 * sigma*sigma));
							}
						destinationImage.at<uchar>(x, y) = saturate_cast<uchar>(value);
					}
			}
		}
		// Khi sourceImage là ảnh màu RGB
		//if (nchannel == 3) {
		//	destinationImage = Mat(height, width, CV_8UC3);

		//	if (method == 0) {
		//		for (int x = 0; x < height; x++)
		//			for (int y = 0; y < width; y++) {
		//				unsigned int value1 = 0, value2 = 0, value3 = 0;

		//				//Tính tích chập bằng toán tử trung bình của 3 kênh
		//				for (int i = -kHeight / 2; i <= kHeight / 2; i++)
		//					for (int j = -kWidth / 2; j <= kWidth / 2; j++) {
		//						value1 = value1 + getFxy(sourceImage, x - i, y - j, 0);
		//						value2 = value2 + getFxy(sourceImage, x - i, y - j, 1);
		//						value3 = value3 + getFxy(sourceImage, x - i, y - j, 2);
		//					}
		//				value1 = value1 / (kWidth*kHeight);
		//				value2 = value2 / (kWidth*kHeight);
		//				value3 = value3 / (kWidth*kHeight);
		//				destinationImage.at<cv::Vec3b>(x, y)[0] = value1;
		//				destinationImage.at<cv::Vec3b>(x, y)[1] = value2;
		//				destinationImage.at<cv::Vec3b>(x, y)[2] = value3;
		//			}
		//	}
		//	else if (method == 1) {
		//		for (int x = 0; x < height; x++)
		//			for (int y = 0; y < width; y++) {
		//				int index_Arr1[maxArrTmp], index_Arr2[maxArrTmp], index_Arr3[maxArrTmp];
		//				int count = 0;
		//				//Tính tích chập và lưu vào 3 mảng chập của 3 kênh
		//				for (int i = -kHeight / 2; i <= kHeight / 2; i++)
		//					for (int j = -kWidth / 2; j <= kWidth / 2; j++) {
		//						index_Arr1[count] = getFxy(sourceImage, x - i, y - j, 0);
		//						index_Arr2[count] = getFxy(sourceImage, x - i, y - j, 1);
		//						index_Arr3[count] = getFxy(sourceImage, x - i, y - j, 2);
		//						count++;
		//					}
		//				//Sắp xếp giá trị 3 mảng chập
		//				for (int i = 0; i < count; i++)
		//					for (int j = i + 1; j < count; j++) {
		//						if (index_Arr1[i] > index_Arr1[j]) {
		//							int tmp = index_Arr1[i];
		//							index_Arr1[i] = index_Arr1[j];
		//							index_Arr1[j] = tmp;
		//						}
		//						if (index_Arr2[i] > index_Arr2[j]) {
		//							int tmp = index_Arr2[i];
		//							index_Arr2[i] = index_Arr2[j];
		//							index_Arr2[j] = tmp;
		//						}
		//						if (index_Arr3[i] > index_Arr3[j]) {
		//							int tmp = index_Arr3[i];
		//							index_Arr3[i] = index_Arr3[j];
		//							index_Arr3[j] = tmp;
		//						}
		//					}
		//				//Lấy giá trị med của cả 3 kênh
		//				destinationImage.at<cv::Vec3b>(x, y)[0] = index_Arr1[count / 2];
		//				destinationImage.at<cv::Vec3b>(x, y)[1] = index_Arr2[count / 2];
		//				destinationImage.at<cv::Vec3b>(x, y)[2] = index_Arr3[count / 2];
		//			}
		//	}
		//	else if (method == 2) {
		//		for (int x = 0; x < height; x++)
		//			for (int y = 0; y < width; y++) {
		//				unsigned int value1 = 0, value2 = 0, value3 = 0;

		//				//Tính tích chập bằng toán tử gaussion
		//				for (int i = -kHeight / 2; i <= kHeight / 2; i++)
		//					for (int j = -kWidth / 2; j <= kWidth / 2; j++) {
		//						value1 = value1 + getFxy(sourceImage, x - i, y - j, 0) * 1 / (sqrt(2 * 3.14)*sigma)*exp(-(i*i + j*j) / (2 * sigma*sigma));
		//						value2 = value2 + getFxy(sourceImage, x - i, y - j, 1) * 1 / (sqrt(2 * 3.14)*sigma)*exp(-(i*i + j*j) / (2 * sigma*sigma));
		//						value3 = value3 + getFxy(sourceImage, x - i, y - j, 2) * 1 / (sqrt(2 * 3.14)*sigma)*exp(-(i*i + j*j) / (2 * sigma*sigma));
		//					}
		//				destinationImage.at<cv::Vec3b>(x, y)[0] = saturate_cast<uchar>(value1);
		//				destinationImage.at<cv::Vec3b>(x, y)[1] = saturate_cast<uchar>(value2);
		//				destinationImage.at<cv::Vec3b>(x, y)[2] = saturate_cast<uchar>(value3);
		//			}
		//	}
		//}

		return 1;
	}
	Blur() {};
	~Blur() {};
};