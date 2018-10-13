#include "stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include<string>
#include"image_handler.h"
using namespace std;
using namespace cv;

typedef vector<Point> Contour;
typedef vector<Vec4i> Hierarchy;

//extern class Pattern_finding;
Image_handler::Image_handler(string path)//
{
	src_image = imread(path, IMREAD_COLOR);
	if (!src_image.data)
	{
		cerr << "读取图片错误" << endl;
	}

}

Image_handler::~Image_handler()
{

}

void Image_handler::convert2black_and_white()
{
	cvtColor(src_image, threshold_image, CV_BGR2GRAY);
	//threshold(threshold_image, threshold_image, 120, 255, CV_THRESH_BINARY);

	double threshold_value=threshold(threshold_image, threshold_image, 0, 255, CV_THRESH_OTSU);
	cout << threshold_value << endl;
	//测试中发现有些轮廓并不连续，滤波后反而好了
	GaussianBlur(threshold_image, threshold_image, Size(5, 5), 0);//需要吗
	Canny(threshold_image, threshold_image, 100, 200);
}

vector<int> Image_handler::get_patterns()
{
	Pattern_finding detector(threshold_image);
	vector<int> Qr_patterns = detector.detect();
	//cout << "patterns size" << Qr_patterns.size() << endl;
	if (!Qr_patterns.empty())
	{
		for (int var : Qr_patterns)
		{
		show_contour(var, src_image, detector);

		}
		cout << "qr size:" << Qr_patterns.size() << endl;
		//show_contour(Qr_patterns[3], src_image, detector);
	}
	else
	{
		cout << "No pattern find" << endl;
		imshow("No pattern", threshold_image);
	}
	return Qr_patterns;

}
void Image_handler::show_image()
{
	imshow("image", threshold_image);
}


