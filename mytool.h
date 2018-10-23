#pragma once
#include"opencv2/opencv.hpp"

using namespace cv;

double cross(Point2f& v1, Point2f& v2);

double calc_slope(const Point2f& pointA, const Point2f& pointB);


double calc_perpendicular_dist(const Point2f& pointA, const Point2f& pointB, const Point2f& pointC, double slope);

double calc_dist(const Point2f& pointA, const Point2f& pointB);

