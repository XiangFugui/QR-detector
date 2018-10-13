#pragma once
#include"stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include<vector>
#include"mytool.h"
#include "pattern_finding.h"
#include "image_handler.h"
using namespace std;
typedef vector<cv::Point> Contour;

void show_rect(int contour_id,cv::Mat& image, Pattern_finding& detector)
{
	cv::Rect box=cv::boundingRect(detector.contours[contour_id]);
	cv::rectangle(image, box, Scalar(0, 0, 255), 2);
	imshow("contour", image);
}

void show_contour(int contour_id, cv::Mat& image, Pattern_finding& detector)
{
	cv::drawContours(image, detector.contours, contour_id, Scalar(0, 0, 255), 2);
	imshow("contour", image);

}

