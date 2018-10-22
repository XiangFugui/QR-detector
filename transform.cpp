#include "stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include"transform.h"



Trans::Trans(Position& position) :TOP(position.Top), RIGHT(position.Right), BOTTOM(position.Bottom)
{

}

Trans::~Trans()
{
}

AffineTrans::AffineTrans(Position& position) :Trans(position)
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