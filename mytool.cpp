#pragma once
#include"stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include<vector>
#include"mytool.h"
#include "image_handler.h"
using namespace std;
typedef vector<cv::Point> Contour;

void show_rect(int contour_id,cv::Mat& image, vector<Contour>& contours)
{
	cv::Rect box=cv::boundingRect(contours[contour_id]);
	cv::rectangle(image, box, Scalar(0, 0, 255), 2);
	imshow("contour", image);
}

void show_contour(int contour_id, cv::Mat& image, vector<Contour>& contours)
{
	cv::drawContours(image, contours, contour_id, Scalar(0, 0, 255), 2);
	imshow("contour", image);

}

