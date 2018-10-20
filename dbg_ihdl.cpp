#include "stdafx.h"
#include<iostream>
#include<vector>
#include"opencv2/opencv.hpp"
#include"image_handler.h"

using namespace std;
using namespace cv;

typedef vector<Vec4i> Hierarchy;
typedef vector<Point> Contour;

enum orientatin { TOP, RIGHT, BOTTOM, LEFT };


void ImageHandler::show_src_image()
{
	imshow("src_image", src_image);
}


void ImageHandler::show_contour_point(int flag)
{
	Mat white;
	if (flag == 0)
	{
		white = src_image.rows, src_image.cols, CV_8UC3, Scalar(255, 255, 255);
	}
	else
	{
		white = src_image;
	}
	for (Point& p : QR_patterns[0])
	{
		circle(white, p, 2, Scalar(0, 0, 255), -1, 8, 0);
	}
	imshow("contour_point", white);
}

void ImageHandler::show_thre_image()
{
	imshow("image", threshold_image);
}
void ImageHandler::show_position()
{

	circle(src_image, position.Bottom, 2, Scalar(0, 0, 255), -1, 8, 0);
	circle(src_image, position.Top, 2, Scalar(255, 0, 0), -1, 8, 0);
	circle(src_image, position.Right, 2, Scalar(0, 255, 0), -1, 8, 0);
	imshow("circle", src_image);
}



void ImageHandler::show_contours(Mat image)
{

	for (size_t index = 0; index < QR_patterns.size(); index++)
	{
		drawContours(image, QR_patterns, index, Scalar(0, 0, 255), 2);
	}
	imshow("contour", image);

}


void ImageHandler::show_contours()
{

	for (size_t index = 0; index < QR_patterns.size(); index++)
	{
		drawContours(src_image, QR_patterns, index, Scalar(0, 0, 255), 2);
	}
	imshow("contour", src_image);
}


void ImageHandler::show_only_contours()
{
	Mat M(src_image.rows, src_image.cols, CV_8UC3, Scalar(255, 255, 255));
	show_contours(M);
}


void ImageHandler::show_corners()
{
	for (int id = 0; id < QR_patterns.size(); id++)
	{
		show_corner(id);
	}
	imshow("circle", src_image);
}


void ImageHandler::show_corner(int id)
{
	//这个方法成立的假设在于描述contour的点中有角点

	//0.8
	Contour test = QR_patterns[id];
	Rect roi = boundingRect(test);

	double x1, x2, y1, y2;
	x1 = roi.tl().x;
	y1 = roi.tl().y;
	x2 = roi.br().x;
	y2 = roi.br().y;
	//roi 和 轮廓所圈面积差不多就以 roi的四角为准了
	double contour_area = contourArea(test);
	double rect_area = roi.area();
	cout << "contour_area" << contour_area << endl;
	cout << "rect_area" << rect_area << endl;
	cout << contour_area / rect_area << endl;

	double X, Y;
	//TOP LEFT BOTTOM LEFT
	double diff[4] = { test[0].y - y1 , x2 - test[0].x , y2 - test[0].y , test[0].x - x1 };
	int temp[4] = { 0,0,0,0 };
	//这里我想了好一会，每个点做四次判断。。也不好做优化
	for (size_t i = 1; i < test.size(); i++)
	{
		X = test[i].x;
		Y = test[i].y;
		if (Y - y1<diff[TOP])
		{
			diff[TOP] = Y - y1;
			temp[TOP] = i;
		}
		if (x2 - X < diff[RIGHT])
		{
			diff[RIGHT] = x2 - X;
			temp[RIGHT] = i;
		}
		if (y2 - Y<diff[BOTTOM])
		{
			diff[BOTTOM] = y2 - Y;
			temp[BOTTOM] = i;
		}
		if (X - x1<diff[LEFT])
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

void ImageHandler::show_rect(int contour_id)
{
	cv::Rect box = cv::boundingRect(QR_patterns[contour_id]);
	cv::rectangle(src_image, box, Scalar(0, 0, 255), 2);
	imshow("rect", src_image);
}