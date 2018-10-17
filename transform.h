#pragma once
#include "determine_orientation.h"
#include"opencv2/opencv.hpp"
#include <iostream>
class Trans
{
public:
	Trans(Position&);
	~Trans();

protected:
	Point point1,point2,point3;
	Mat trans_image;


};

Trans::Trans(Position& position):point1(position.Top), point2(position.Right),point3(position.Bottom)
{
	
}

Trans::~Trans()
{
}

class AffineTrans:public Trans
{
public:
	AffineTrans(Position&);
	~AffineTrans();
	Mat& transform(Mat& image);

private:
	
};

AffineTrans::AffineTrans(Position& position):Trans(position)
{
}

AffineTrans::~AffineTrans()
{
}
Mat& AffineTrans::transform(Mat& image)
{
	/*vector<Point> src = { point1,point2,point3 };
	vector<Point> dst = {Point(40,40),Point(140,40),Point(40,140)};	*/
	Point2f src[3] = { point1,point2,point3 };
	Point2f dst[3];
	dst[0] = Point(40, 40);
	dst[1] = Point(140, 40);
	dst[2] = Point(40, 140);

	Mat warp_matrix=getAffineTransform(src,dst);

	//Mat qr(100, 100, CV_8UC3);
	warpAffine(image, trans_image, warp_matrix, Size(200, 200));
	trans_image.resize((200,200));
	return trans_image;
}