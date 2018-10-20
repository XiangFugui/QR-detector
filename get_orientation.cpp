#pragma once
#include "stdafx.h"
#include<vector>
#include"opencv2/opencv.hpp"
#include"get_orientation.h"
#include<iostream>
using namespace std;
using namespace cv;
GetOrientation::GetOrientation(Contour& contourA, Contour& contourB, Contour& contourC)
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


Position& GetOrientation::find_orientation()
{
	double dist_AB = calc_dist(point_A, point_B);
	double dist_AC = calc_dist(point_A, point_C);
	double dist_BC = calc_dist(point_B, point_C);


	double longest = max(dist_AB, max(dist_AC, dist_BC));

	if (dist_AB==longest)
	{
		get_points_relation(point_A, point_B, point_C);
	}
	if (dist_AC==longest)
	{
		get_points_relation(point_A, point_C, point_B);
	}
	if (dist_BC==longest)
	{
		get_points_relation(point_B, point_C, point_A);
	}

	return position;
}


void GetOrientation::get_points_relation(const Point2f& median1, const Point2f& median2, const Point2f& outlier)
{
	//这两个函数有问题
	double slope = calc_slope(median1, median2);
	double ppd_dist = calc_perpendicular_dist(median1, median2, outlier, slope);

	position.Top = outlier;
	cout << "slope" << slope<< endl;
	cout << "dist" << ppd_dist<<endl;
	if (ppd_dist == 0)
	{
		//TO DO
		cerr << "poisition 在一条支线上" << endl;
		exit(0);
	}
	if (slope < 0 )       
	{
		//回    回
		//
		//回
		if (ppd_dist < 0)
		{
			if (median1.y > median2.y)
			{
				position.Bottom = median1;
				position.Right = median2;
			}
			else
			{
				position.Bottom = median2;
				position.Right = median1;

			}
		}
		//     回
		//
		//回	   回
		else
		{
			if (median1.y > median2.y)
			{
				position.Right = median1;
				position.Bottom = median2;
			}
			else
			{
				position.Right = median2;
				position.Bottom = median1;
			}
		}
	}
	else
	{
		//回
		//
		//回   回
		if (ppd_dist >= 0)
		{
			if (median1.x > median2.x)
			{
				position.Bottom = median1;
				position.Right = median2;
			}
			else
			{
				position.Bottom = median2;
				position.Right = median1;
			}
		}
		else
		{
		//回  回
		//
		//    回
			if (median1.x > median2.x)
			{
				position.Bottom = median2;
				position.Right = median1;
			}
			else
			{
				position.Bottom = median1;
				position.Right = median2;
			}
		}
	}
}


double GetOrientation::calc_slope(const Point2f& pointA, const Point2f& pointB)
{
	double temp = pointB.x - pointA.x;
	if (temp == 0)
		return temp;
	double result = (pointB.y - pointA.y) /temp;
	return result;
}


double GetOrientation::calc_perpendicular_dist(const Point2f& pointA, const Point2f& pointB, const Point2f& pointC, double slope)
{
	double a, b, c, dist;
	a = slope * (-1);
	b = 1;
	c = (slope*pointB.x) - pointB.y;
	dist = (a*pointC.x + b * pointC.y + c) / sqrt((a*a) + (b*b));
	return dist;
}

double GetOrientation::calc_dist(const Point2f& pointA, const Point2f& pointB)
{
	return sqrt(pow(pointA.x - pointB.x, 2) + pow(pointA.y - pointB.y, 2));
}

GetOrientation::~GetOrientation()
{

}