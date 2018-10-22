#pragma once
#include"opencv2/opencv.hpp"
#include"image_handler.h"
#include "get_orientation.h"
#include <iostream>
#include <numeric>

#define top_cnt position.Top;
#define bottom_cnt position.Bottom;

typedef vector<Point> Contour;

extern orientation ORIENTATION;
extern Position position;

class Trans
{
public:
	Trans(Position&);
	~Trans();

protected:
	Point2f TOP,RIGHT,BOTTOM;
};



class AffineTrans
{
public:
	AffineTrans(Position&);
	~AffineTrans();
	Mat& transform();

private:
	Point2f TOP, RIGHT, BOTTOM;

};

class PerspectiveTrans
{
public:
	PerspectiveTrans(Position& position);
	~PerspectiveTrans();
	Mat& transform(Mat& image);
private:
	Point2f& find_4th(Contour& cRight, Contour& Bottom);
	void find_coners(int id);
};


PerspectiveTrans::PerspectiveTrans(Position& position):
{
}

PerspectiveTrans::~PerspectiveTrans()
{
}

Mat& PerspectiveTrans::transform(Mat& image);
{

}


Point2f& PerspectiveTrans::find_4th(Contour& cRight, Contour& )
{

}

float mean(vector<int>& vec)
{
	int sum = accumulate(vec.begin(), vec.end(), 0);
	double temp = (sum / vec.size());
	return temp;
}

void PerspectiveTrans::find_coners(int id)
{
	//这个方法成立的假设在于描述contour的点中有角点

	//实际上倾斜量在3-7度以内的，这两种方法都不是很精确的
	//尴尬的角度
	Contour test = QR_patterns[id];
	Rect roi = boundingRect(test);

	int x1, x2, y1, y2;

	x1 = roi.tl().x;
	y1 = roi.tl().y;
	x2 = roi.br().x;
	y2 = roi.br().y;

	double area_cnt = contourArea(test);
	double area_rect = roi.area();
	double ratio = area_cnt / area_rect;
	cout << "ratio" << ratio << endl;

	if (ratio>0.85)
	{
		Point2f pointA(x1, y1);
		Point2f pointB(x2, y1);
		Point2f pointC(x2, y2);
		Point2f pointD(x1, y2);
		Point2f points[] = { pointA ,pointB ,pointC,pointD}
	}
	else
	{
		int X, Y;

		vector<int> top_corners;
		vector<int> right_corners;
		vector<int> bottom_corners;
		vector<int> left_corners;

		for (size_t i = 0; i < test.size(); i++)
		{
			X = test[i].x;
			Y = test[i].y;

			int diff_y1 = Y - y1;
			int diff_y2 = y2 - Y;
			int diff_x1 = X - x1;
			int diff_x2 = x2 - X;

			//2这个阈值虽然很可能出错，但确实很精确了
			int thre_val = 2;
			if (diff_y1<thre_val)
			{
				top_corners.push_back(test[i].x);
			}
			if (diff_x2 <thre_val)
			{
				right_corners.push_back(test[i].y);
			}
			if (diff_y2 <thre_val)
			{
				bottom_corners.push_back(test[i].x);
			}
			if (diff_x1<thre_val)
			{
				left_corners.push_back(test[i].y);
			}
		}
		Point2f pointA(mean(top_corners), y1);
		Point2f pointB(x2, mean(right_corners));
		Point2f pointC(mean(bottom_corners), y2);
		Point2f pointD(x1, mean(left_corners));
	}
}