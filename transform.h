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
	Point2f TOP,RIGHT,BOTTOM;
	Mat trans_image;
};

Trans::Trans(Position& position):TOP(position.Top), RIGHT(position.Right),BOTTOM(position.Bottom)
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
	Point2f src[3] = { TOP,RIGHT,BOTTOM };
	Point2f dst[3];
	dst[0] = Point(40, 40);
	dst[1] = Point(140, 40);
	dst[2] = Point(40, 140);
	cout << TOP.x << endl;
	cout << RIGHT.x << endl;
	cout << BOTTOM.x << endl;
	Mat warp_matrix=getAffineTransform(src,dst);
	//Mat qr(100, 100, CV_8UC3);
	warpAffine(image, trans_image, warp_matrix, Size(200, 200));
	//trans_image.resize((200,200));
	cout << trans_image.size() << endl;
	return trans_image;
}