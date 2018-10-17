#pragma once
#include "stdafx.h"
#include<vector>
#include"opencv2/opencv.hpp"
#include"determine_orientation.h"
#include<iostream>
using namespace std;
using namespace cv;
Determine_orientation::Determine_orientation(Contour& contourA, Contour& contourB, Contour& contourC)
{
	Moments momentA = moments(contourA);
	Moments momentB = moments(contourB);
	Moments momentC = moments(contourC);
	point_A.x = (momentA.m10 / momentA.m00);
	point_A.y = (momentA.m01 / momentA.m00);

	point_B.x = (momentB.m10 / momentB.m00);
	point_B.y = (momentB.m01 / momentB.m00);

	point_C.x = (momentC.m10 / momentC.m00);
	point_C.y = (momentC.m01 / momentC.m00);
}


Position& Determine_orientation::find_orientation()
{
	/*double slope_AB = calc_slope(point_A, point_B);
	double slope_AC = calc_slope(point_A, point_C);
	double slope_BC = calc_slope(point_B, point_C);*/

	double dist_AB = calc_dist(point_A, point_B);
	double dist_AC = calc_dist(point_A, point_C);
	double dist_BC = calc_dist(point_B, point_C);

	if (dist_AB > dist_BC && dist_AB > dist_AC)
	{
		//outlier = C; median1 = A; median2 = B;
		get_points_relation(point_C, point_A, point_B);
	}
	else if (dist_AC > dist_AB && dist_AC > dist_BC)
	{
		//outlier = B; median1 = A; median2 = C;
		get_points_relation(point_B, point_A, point_C);

	}
	else if (dist_BC > dist_AB && dist_BC > dist_AC)
	{
		//outlier = A;  median1 = B; median2 = C;
		get_points_relation(point_A, point_B, point_C);
	}

	return position;
}


void Determine_orientation::get_points_relation(const Point2f& median1, const Point2f& median2, const Point2f& outlier)
{
	//这两个函数有问题
	double slope = calc_slope(median1, median2);

	double ppd_dist = calc_perpendicular_dist(median1, median2, outlier, slope);

	position.Top = outlier;
	cout << "slope" << slope<< endl;
	cout << "dist" << ppd_dist<<endl;
	if (ppd_dist == 0)
	{
		cerr << "poisition 在一条支线上" << endl;
		exit(0);
	}
	if (slope <= 0 && ppd_dist < 0)        // Orientation - North
	{
		position.Bottom = median1;
		position.Right = median2;
		//orientation = CV_QR_NORTH;
		cout << "North" << endl;
	}
	else if (slope > 0 && ppd_dist < 0)        // Orientation - East
	{
		position.Right = median1;
		position.Bottom = median2;
		//orientation = CV_QR_EAST;
		cout << "East" << endl;

	}
	else if (slope < 0 && ppd_dist > 0)        // Orientation - South          
	{
		position.Right = median1;
		position.Bottom = median2;
		//orientation = CV_QR_SOUTH;
		cout << "South" << endl;
	}

	else if (slope > 0 && ppd_dist > 0)        // Orientation - West
	{
		position.Bottom = median1;
		position.Right = median2;
		//orientation = CV_QR_WEST;
		cout << "West" << endl;
	}
	//else if (slope==0)
	//{
	//	position.Bottom = median1;
	//	position.Right = median2;
	//	//orientation = CV_QR_NORTH;
	//	cout << "???" << endl;
	//}

}


double Determine_orientation::calc_slope(const Point2f& pointA, const Point2f& pointB)
{
	double temp = pointB.x - pointA.x;
	if (temp == 0)
		return temp;
	double result = (pointB.y - pointA.y) /temp;
	return result;
}


double Determine_orientation::calc_perpendicular_dist(const Point2f& pointA, const Point2f& pointB, const Point2f& pointC, double slope)
{
	double a, b, c, dist;
	a = slope * (-1);
	b = 1;
	c = (slope*pointB.x) - pointB.y;
	dist = (a*pointC.x + b * pointC.y + c) / sqrt((a*a) + (b*b));
	return dist;
}

double Determine_orientation::calc_dist(const Point2f& pointA, const Point2f& pointB)
{
	return sqrt(pow(pointA.x - pointB.x, 2) + pow(pointA.y - pointB.y, 2));
}

Determine_orientation::~Determine_orientation()
{

}