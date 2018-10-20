#pragma once
#include"opencv2/opencv.hpp"
#include"image_handler.h"
#include "get_orientation.h"

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



class AffineTrans:public Trans
{
public:
	AffineTrans(Position&);
	~AffineTrans();
	Mat& transform(Mat& image);

private:
	
};

