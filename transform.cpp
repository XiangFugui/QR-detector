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

PerspectiveTrans::PerspectiveTrans(Position& position) :top_cnt(position.Top), right_cnt(position.Right), bottom_cnt(position.Bottom)
{
	find_coners(position.Top, top_quad);
	find_coners(position.Right, right_quad);
	find_coners(position.Bottom, bottom_quad);
}

PerspectiveTrans::~PerspectiveTrans()
{
}



float mean(vector<int>& vec)
{
	int sum = accumulate(vec.begin(), vec.end(), 0);
	return (sum / vec.size());
}

void PerspectiveTrans::find_coners(Contour& contour, Quad& quad)
{
	//这个方法成立的假设在于描述contour的点中有角点

	//实际上倾斜量在3-7度以内的，这两种方法都不是很精确的
	//尴尬的角度
	Rect roi = boundingRect(contour);

	int x1, x2, y1, y2;

	x1 = roi.tl().x;
	y1 = roi.tl().y;
	x2 = roi.br().x;
	y2 = roi.br().y;

	double area_cnt = contourArea(contour);
	double area_rect = roi.area();
	double ratio = area_cnt / area_rect;
	cout << "ratio" << ratio << endl;

	if (ratio>0.85)
	{
		/*Point2f pointtl(x1, y1);
		Point2f pointB(x2, y1);
		Point2f pointC(x2, y2);
		Point2f pointD(x1, y2);
		Point2f points[] = { pointtl ,pointB ,pointC,pointD };*/
		quad.tl = Point2f(x1, y1);
		quad.tr = Point2f(x2, y1);
		quad.br = Point2f(x2, y2);
		quad.bl = Point2f(x1, y2);
	}
	else
	{
		int X, Y;

		vector<int> top_corners;
		vector<int> right_corners;
		vector<int> bottom_corners;
		vector<int> left_corners;

		for (size_t i = 0; i < contour.size(); i++)
		{
			X = contour[i].x;
			Y = contour[i].y;

			int diff_y1 = Y - y1;
			int diff_y2 = y2 - Y;
			int diff_x1 = X - x1;
			int diff_x2 = x2 - X;

			//2这个阈值虽然很可能出错，但确实很精确了
			int thre_val = 2;
			if (diff_y1<thre_val)
			{
				top_corners.push_back(contour[i].x);
			}
			if (diff_x2 <thre_val)
			{
				right_corners.push_back(contour[i].y);
			}
			if (diff_y2 <thre_val)
			{
				bottom_corners.push_back(contour[i].x);
			}
			if (diff_x1<thre_val)
			{
				left_corners.push_back(contour[i].y);
			}
		}
		/*Point2f pointA(mean(top_corners), y1);
		Point2f pointB(x2, mean(right_corners));
		Point2f pointC(mean(bottom_corners), y2);
		Point2f pointD(x1, mean(left_corners));*/

		quad.tl = Point2f(mean(top_corners), y1);
		quad.tr = Point2f(x2, mean(right_corners));
		quad.br = Point2f(mean(bottom_corners), y2);
		quad.bl = Point2f(x1, mean(left_corners));
	}
}

Mat& PerspectiveTrans::transform(Mat& image)
{
	return image;
}

Quad PerspectiveTrans::detemine_corners()
{
	return top_quad;

}
Point2f PerspectiveTrans::find_4th()
{
	Point2f target;
	cout << "ORIENTATION" << ORIENTATION << endl;

	switch (ORIENTATION)
	{
	case NorthWest:
		target = get_intersection_point(right_quad.tr, right_quad.br, bottom_quad.br, bottom_quad.bl);
		break;

	case SouthEast:
		target = get_intersection_point(right_quad.tl, right_quad.bl, bottom_quad.tl, bottom_quad.tr);
		break;
	case SouthWest:

		target = get_intersection_point(right_quad.tl, right_quad.tr, bottom_quad.tr, bottom_quad.br);
		break;

	case NorthEast:
		target = get_intersection_point(right_quad.br, right_quad.bl, bottom_quad.tl, bottom_quad.bl);
		break;

	default:
		break;
	}
	cout << "target.x "<< target.x << endl;
	return target;
}

Point2f PerspectiveTrans::get_intersection_point(Point2f a1, Point2f& a2, Point2f& b1, Point2f& b2)
{

	Point2f r(a2 - a1);
	Point2f s(b2 - b1);
	Point2f t(a1 - b1);
	double ratio = cross(t, s) / cross(r, s);
	Point2f intersection = ratio * r + a1;
	return intersection;
}