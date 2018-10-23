#include "stdafx.h"
#include<iostream>
#include<vector>
#include"opencv2/opencv.hpp"
#include"image_handler.h"

using namespace std;
using namespace cv;

typedef vector<Vec4i> Hierarchy;
typedef vector<Point> Contour;



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
void ImageHandler::show_position(Position& position)
{

	circle(src_image, position.Bottom_mass_point, 2, Scalar(0, 0, 255), -1, 8, 0);
	circle(src_image, position.Top_mass_point, 2, Scalar(255, 0, 0), -1, 8, 0);
	circle(src_image, position.Right_mass_point, 2, Scalar(0, 255, 0), -1, 8, 0);
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

//float mean(vector<int>& vec)
//{
//	int sum = accumulate(vec.begin(), vec.end(), 0);
//	double temp = (sum / vec.size());
//	return temp;
//}
void ImageHandler::show_corner(int id)
{
	//这个方法成立的假设在于描述contour的点中有角点

	//实际上倾斜量在3-7度以内的，这两种方法都不是很精确的
	//尴尬的角度
	
}

void ImageHandler::show_rect(int contour_id)
{
	cv::Rect box = cv::boundingRect(QR_patterns[contour_id]);
	cv::rectangle(src_image, box, Scalar(127, 127, 127), 1);
	imshow("rect", src_image);
}

