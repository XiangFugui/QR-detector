#pragma once
#include"stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include<vector>
#include"mytool.h"
//#include "image_handler.h"
using namespace std;
typedef vector<Point2f> Contour;

double cross(Point2f& v1, Point2f& v2)
{
	return (v1.x*v2.y - v1.y*v2.x);
}
double calc_slope(const Point2f& pointA, const Point2f& pointB)
{
	double temp = pointB.x - pointA.x;
	if (temp == 0)
		return temp;
	double result = (pointB.y - pointA.y) / temp;
	return result;
}


double calc_perpendicular_dist(const Point2f& pointA, const Point2f& pointB, const Point2f& pointC, double slope)
{
	double a, b, c, dist;
	a = slope * (-1);
	b = 1;
	c = (slope*pointB.x) - pointB.y;
	dist = (a*pointC.x + b * pointC.y + c) / sqrt((a*a) + (b*b));
	return dist;
}

double calc_dist(const Point2f& pointA, const Point2f& pointB)
{
	return sqrt(pow(pointA.x - pointB.x, 2) + pow(pointA.y - pointB.y, 2));
}

