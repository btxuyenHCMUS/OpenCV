#pragma once
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <vector>

using namespace cv;
using namespace std;

class Convolution
{
	vector<float> _kernel;// lưu giá trị của ma trận kernel dùng để tích chập
	int _kernelWidth;//chiều rộng kernel
	int _kernelHeight;//chiều dài kernel
public:
	//trả về kernel 
	vector<float> GetKernel() {
		return _kernel;
	}
	//set kernel, chiều rộng, chiều dài kernel phải là số lẻ
	void SetKernel(vector<float> kernel, int kWidth, int kHeight) {
		this->_kernelHeight = kHeight;
		this->_kernelWidth = kWidth;
		for (int ii = 0; ii < kHeight*kWidth; ii++) {
			this->_kernel.push_back(kernel[ii]);
		}
	}

	/*
	Hàm tính convolution của 1 ảnh xám với kernel được xác định trước
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu tính thành công
	1: nếu tính thất bại (không đọc được ảnh input,...)
	*/
	int DoConvolution(const Mat& sourceImage, Mat& destinationImage) {
		//Nếu ảnh vào không có dữ liệu hoặc là ảnh màu thì thoát(0)...
		if (sourceImage.data == NULL) return 0;
		if (sourceImage.channels() == 3) return 0;
		int height = sourceImage.rows, width = sourceImage.cols;
		//Tạp mảng index kernel đễ dễ truy xuất kernel
		int _kernelIndexRow[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
		int _kernelIndexCol[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
		
		destinationImage = Mat(height, width, CV_8UC1);

		for (int x = 0; x < height; x++)
			for (int y = 0; y < width; y++) {
				unsigned int cvlt_value = 0;
				for (int ii = 0; ii < _kernel.size(); ii++) {
					//Nếu vị trí nằm ngoài ảnh thì không xét
					if (x - _kernelIndexRow[ii] < 0 || x - _kernelIndexRow[ii] >= height)
						continue;
					if (y - _kernelIndexCol[ii] < 0 || y - _kernelIndexCol[ii] >= width)
						continue;
					//Thực hiện tích chập bằng phép tổng các tích
					cvlt_value += _kernel[ii] * sourceImage.at<uchar>(x - _kernelIndexRow[ii], y - _kernelIndexCol[ii]);
				}
				//Gán giá trị tích chập
				destinationImage.at<uchar>(x, y) = saturate_cast<uchar>(cvlt_value);
			}

		return 1;
	}
	Convolution() {};
	~Convolution() {};
};

