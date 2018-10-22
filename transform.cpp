#include "stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include"transform.h"



AffineTrans::AffineTrans(Position& position) :TOP(position.Top_mass_point), RIGHT(position.Right_mass_point), BOTTOM(position.Bottom_mass_point)
{
}

AffineTrans::~AffineTrans()
{
}
Mat& AffineTrans::transform()
{
	Point2f src[3] = { TOP,RIGHT,BOTTOM };
	Point2f dst[3];
	dst[0] = Point(40, 40);
	dst[1] = Point(140, 40);
	dst[2] = Point(40, 140);
	cout << TOP.x << endl;
	cout << RIGHT.x << endl;
	cout << BOTTOM.x << endl;
	Mat warp_matrix = getAffineTransform(src, dst);
	return warp_matrix;
}