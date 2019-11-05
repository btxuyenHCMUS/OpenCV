#pragma once
#include <math.h>
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"

#define		RGB2Gray	0
#define		Gray2RGB	1
#define		RGB2toHSV	2
#define		HSVto2RGB	3

using namespace cv;

int maximum(int num1, int num2, int num3);
int minimize(int num1, int num2, int num3);
void RGBtoHSV(int r, int g, int b, float &h, float &s, float &v);

class Converter
{
	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang GrayScale
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
public:
	int RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage);

	/*
	Hàm chuyển đổi không gian màu của ảnh từ GrayScale sang RGB
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage);
	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang HSV
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int RGB2HSV(const Mat& sourceImage, Mat& destinationImage);
	/*
	Hàm chuyển đổi không gian màu của ảnh từ HSV sang RGB
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int HSV2RGB(const Mat& sourceImage, Mat& destinationImage);

public:
	/*
	Hàm chuyển đổi không gian màu của ảnh
	sourceImage: ảnh input
	destinationImage: ảnh output cùng kích thước, cùng loại với ảnh input
	type: loại chuyển đổi
	0,1: chuyển từ RGB sang GrayScale và ngược lại
	2,3: chuyển từ RGB sang HSV và ngược lại
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input hay type không chính xác,...)
	*/
	int Convert(Mat& sourceImage, Mat& destinationImage, int type);
	int Convert(IplImage* sourceImage, IplImage* destinationImage, int type);
	Converter();
	~Converter();
};