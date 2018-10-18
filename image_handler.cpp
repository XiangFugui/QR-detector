#include "stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include<string>
#include"image_handler.h"
#include"determine_orientation.h"
#include"transform.h"
using namespace std;
using namespace cv;

typedef vector<Point> Contour;
typedef vector<Vec4i> Hierarchy;

//extern class PatternFinding;
extern void m_show_contours(cv::Mat& image, vector<Contour>& contours);
ImageHandler::ImageHandler(string path)//
{
	src_image = imread(path, IMREAD_COLOR);
	if (!src_image.data)
	{
		cerr << "读取图片错误" << endl;
	}
}

ImageHandler::~ImageHandler()
{

}

void ImageHandler::convert2black_and_white()
{
	cvtColor(src_image, threshold_image, CV_BGR2GRAY);
	//threshold(threshold_image, threshold_image, 120, 255, CV_THRESH_BINARY);

	double threshold_value=threshold(threshold_image, threshold_image, 0, 255, CV_THRESH_OTSU);
	//测试中发现有些轮廓并不连续，滤波后反而好了.
	//有些图片肉眼观察正常，但识别不出来，把kernel由(5,5)->(3,3)就好了
	GaussianBlur(threshold_image, threshold_image, Size(3, 3), 0);//需要吗
	Canny(threshold_image, threshold_image, 100, 200);
}

vector<Contour> ImageHandler::get_patterns()
{
	PatternFinding detector(threshold_image);
	QR_patterns = detector.detect();
	//cout << "patterns size" << Qr_patterns.size() << endl;
	//show_contours(src_image, QR_patterns);

	
	return QR_patterns;
}

Position& ImageHandler::QR_in_image()
{
	Determine_orientation orientation_obj(QR_patterns[0], QR_patterns[1], QR_patterns[2]);
	position = orientation_obj.find_orientation();
	return position;
}

Mat& ImageHandler::transform()
{
	AffineTrans trans_obj(position);
	QR_image=trans_obj.transform(src_image);
	imshow("QR_trans", QR_image);
	return QR_image;
}

void ImageHandler::show_image()
{
	imshow("image", threshold_image);
}
void ImageHandler::show_position()
{
	
	circle(src_image, position.Bottom, 2, Scalar(0, 0,255), -1, 8, 0);
	circle(src_image, position.Top, 2, Scalar(255,0, 0), -1, 8, 0);
	circle(src_image, position.Right, 2, Scalar(0, 255,0), -1, 8, 0);
	imshow("circle", src_image);
}
void ImageHandler::show_contours()
{
	if (QR_patterns.size() == 3)
	{
		m_show_contours(src_image, QR_patterns);
		//cout << "qr size:" << QR_patterns.size() << endl;
	}
	else
	{
		cout << "No pattern find" << endl;
		imshow("No pattern", threshold_image);
	}
}
