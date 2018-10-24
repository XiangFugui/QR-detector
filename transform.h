#pragma once
#include"opencv2/opencv.hpp"
#include"image_handler.h"
#include "get_orientation.h"
#include <iostream>
#include <numeric>
#include"mytool.h"

//好不容易在这里成功了
extern enum orientation { NorthWest, SouthEast, SouthWest, NorthEast };
extern orientation ORIENTATION;
extern Position position;

typedef vector<Point> Contour;

class AffineTrans
{
public:
	AffineTrans(Position&);
	~AffineTrans();
	Mat transform(Mat src_image);

private:
	Point2f TOP, RIGHT, BOTTOM;

};

struct Quad
{
	Point2f tl;
	Point2f tr;
	Point2f br;
	Point2f bl;
};

class PerspectiveTrans
{
public:
	PerspectiveTrans(Position& position);
	~PerspectiveTrans();
	Mat transform(Mat image);
public:
	Point2f find_4th();
	void find_corners(Contour& contour, Quad& quad);
	Point2f get_intersection_point(Point2f a1, Point2f& a2, Point2f& b1, Point2f& b2);
	Contour top_cnt, right_cnt, bottom_cnt;
	Quad top_quad, right_quad, bottom_quad;
};


