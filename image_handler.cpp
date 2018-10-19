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

enum orientatin{TOP,RIGHT,BOTTOM,LEFT};
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
	//if (QR_patterns.size() == 3)
	//{
	//	m_show_contours(src_image, QR_patterns);
	//	//cout << "qr size:" << QR_patterns.size() << endl;
	//}
	//else
	//{
	//	cout << "No pattern find" << endl;
	//	imshow("No pattern", threshold_image);
	//}
	m_show_contours(src_image, QR_patterns);

}


void ImageHandler::show_center()
{
	cout << QR_patterns[0].size() << endl;
	for (Point& p : QR_patterns[0])
	{
		circle(src_image, p, 2, Scalar(0, 0, 255), -1, 8, 0);
	}
	imshow("test", src_image);
}


void ImageHandler::show_corner(int id)
{
	//这个方法成立的假设在于描述contour的点中有角点

	Contour test = QR_patterns[id];
	Rect roi = boundingRect(test);
	//cv::rectangle(src_image, roi, Scalar(128, 0, 255), 2);
	/*Mat imageROI(src_image, roi);
	imshow("corner", imageROI);*/
	double x1, x2, y1, y2;
	x1 = roi.tl().x;
	y1 = roi.tl().y;
	x2 = roi.br().x;
	y2 = roi.br().y;

	double X, Y;
	//TOP LEFT BOTTOM LEFT
	double diff[4] = {test[0].y-y1 , x2-test[0].x , y2- test[0].y , test[0].x-x1 };
	int temp[4] = { 0,0,0,0 };
	//这里我想了好一会，每个点做四次判断。。也不好做优化
	for (size_t i = 1; i < test.size(); i++)
	{
		X = test[i].x;
		Y = test[i].y;
		if (Y-y1<diff[TOP])
		{
			diff[TOP] = Y - y1;
			temp[TOP] = i;
		}
		if (x2 - X < diff[RIGHT])
		{
			diff[RIGHT] = x2 - X;
			temp[RIGHT] = i;
		}
		if (y2-Y<diff[BOTTOM])
		{
			diff[BOTTOM] = y2 - Y;
			temp[BOTTOM] = i;
		}
		if (X-x1<diff[LEFT])
		{
			diff[LEFT] = X - x1;
			temp[LEFT] = i;
		}
	}
	circle(src_image, test[temp[TOP]], 2, Scalar(0, 0, 255), -1, 8, 0);
	circle(src_image, test[temp[RIGHT]], 2, Scalar(0, 0, 255), -1, 8, 0);
	circle(src_image, test[temp[BOTTOM]], 2, Scalar(0, 0, 255), -1, 8, 0);
	circle(src_image, test[temp[LEFT]], 2, Scalar(0, 0, 255), -1, 8, 0);
}

void ImageHandler::show_corners()
{
	for (int id = 0; id < QR_patterns.size(); id++)
	{
		show_corner(id);
	}
	imshow("circle", src_image);

}