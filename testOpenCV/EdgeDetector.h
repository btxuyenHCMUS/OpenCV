#pragma once
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"

using namespace cv;

unsigned char getFxy(const Mat& sourceImage, int x, int y)
{
	if (x < 0 || x >= sourceImage.rows)
		return 0;
	if (y < 0 || y >= sourceImage.cols)
		return 0;
	return sourceImage.at<uchar>(x, y);
}

class EdgeDetector
{
public:
	/*
	Hàm phát hiện biên cạnh của ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel
	1: Sobel
	2: Prewitt
	3: Prei-Chen
	4: Laplace
	Hàm trả về
	0: nếu detect thành công
	1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int DetectEdge(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method) {
		if (sourceImage.data == NULL) return 0;
		int height = sourceImage.rows, width = sourceImage.cols;

		//phát hiện biên chỉ dành cho ảnh xám
		if (sourceImage.channels() == 1) {
			destinationImage = Mat(height, width, CV_8UC1);
			//Nếu dùng phưuong pháp sobel, thì tính gradient với k = 2
			if (method == 1) {
				//
				float k = 1;
				for (int x = 0; x < height; x++)
					for (int y = 0; y < width; y++) {
						float fx = 1 / (k+2) * ((getFxy(sourceImage, x - 1, y + 1) + k * getFxy(sourceImage, x, y + 1) + getFxy(sourceImage, x + 1, y + 1)) - (getFxy(sourceImage, x - 1, y - 1) + k * getFxy(sourceImage, x, y - 1) + getFxy(sourceImage, x + 1, y - 1)));
						float fy = 1 / (k+2) * ((getFxy(sourceImage, x - 1, y - 1) + k * getFxy(sourceImage, x - 1, y) + getFxy(sourceImage, x - 1, y + 1)) - (getFxy(sourceImage, x + 1, y - 1) + k * getFxy(sourceImage, x + 1, y) + getFxy(sourceImage, x + 1, y + 1)));
						int G = sqrt(fx*fx + fy*fy);
						destinationImage.at<uchar>(x, y) = saturate_cast<uchar>(G);
					}
			}
			//Nếu dùng phương pháp prewitt, thì graident với k = 1
			else if (method == 2) {
				//
				float k = 1;
				for (int x = 0; x < height; x++)
					for (int y = 0; y < width; y++) {
						float fx = 1 / (k + 2) * ((getFxy(sourceImage, x - 1, y + 1) + k * getFxy(sourceImage, x, y + 1) + getFxy(sourceImage, x + 1, y + 1)) - (getFxy(sourceImage, x - 1, y - 1) + k * getFxy(sourceImage, x, y - 1) + getFxy(sourceImage, x + 1, y - 1)));
						float fy = 1 / (k + 2) * ((getFxy(sourceImage, x - 1, y - 1) + k * getFxy(sourceImage, x - 1, y) + getFxy(sourceImage, x - 1, y + 1)) - (getFxy(sourceImage, x + 1, y - 1) + k * getFxy(sourceImage, x + 1, y) + getFxy(sourceImage, x + 1, y + 1)));
						int G = sqrt(fx*fx + fy*fy);
						destinationImage.at<uchar>(x, y) = saturate_cast<uchar>(G);
					}
			}
			//Nếu dùng phương pháp prei-chen, thì tính graident với k = căn2
			else if (method == 3) {
				//
				float k = sqrt(2);
				for (int x = 0; x < height; x++)
					for (int y = 0; y < width; y++) {
						float fx = 1 / (k + 2) * ((getFxy(sourceImage, x - 1, y + 1) + k * getFxy(sourceImage, x, y + 1) + getFxy(sourceImage, x + 1, y + 1)) - (getFxy(sourceImage, x - 1, y - 1) + k * getFxy(sourceImage, x, y - 1) + getFxy(sourceImage, x + 1, y - 1)));
						float fy = 1 / (k + 2) * ((getFxy(sourceImage, x - 1, y - 1) + k * getFxy(sourceImage, x - 1, y) + getFxy(sourceImage, x - 1, y + 1)) - (getFxy(sourceImage, x + 1, y - 1) + k * getFxy(sourceImage, x + 1, y) + getFxy(sourceImage, x + 1, y + 1)));
						int G = sqrt(fx*fx + fy*fy);
						destinationImage.at<uchar>(x, y) = saturate_cast<uchar>(G);
					}
			}
			//Nếu dùng phương pháp laplace
			else if (method == 4) {
				for (int x = 0;x<height;x++)
					for (int y = 0; y < width; y++) {
						int value = getFxy(sourceImage, x + 1, y) + getFxy(sourceImage, x - 1, y) + getFxy(sourceImage, x, y + 1) + getFxy(sourceImage, x, y - 1) - 4 * getFxy(sourceImage, x, y);
						destinationImage.at<uchar>(x, y) = saturate_cast<uchar>(value);
					}
			}
		}

		return 1;
	}
	EdgeDetector() {}
	~EdgeDetector() {}
};

