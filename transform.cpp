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

Mat AffineTrans::transform(Mat src_image)
{
	Point2f src[3] = { TOP,RIGHT,BOTTOM };
	Point2f dst[3];
	//dst[0] = Point(40, 40);
	//dst[1] = Point(140, 40);
	//dst[2] = Point(40, 140);

	/*dst[0] = Point(40, 40);
	dst[1] = Point(140, 40);
	dst[2] = Point(40, 140);*/
	dst[0] = Point(20, 20);
	dst[1] = Point(120, 20);
	dst[2] = Point(20, 120);


	cout << TOP.x << endl;
	cout << RIGHT.x << endl;
	cout << BOTTOM.x << endl;
	Mat warp_matrix = getAffineTransform(src, dst);
	Mat QR_image;
	warpAffine(src_image, QR_image, warp_matrix, Size(200, 200));
	return QR_image;
}

PerspectiveTrans::PerspectiveTrans(Position& position)
{
	find_corners(position.Top, top_quad);
	find_corners(position.Right, right_quad);
	find_corners(position.Bottom, bottom_quad);
}

PerspectiveTrans::~PerspectiveTrans()
{
}



float mean(vector<int>& vec)
{
	int sum = accumulate(vec.begin(), vec.end(), 0);
	return (sum / vec.size());
}

void PerspectiveTrans::find_corners(Contour& contour, Quad& quad)
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

		quad.tl = Point2f(mean(top_corners), y1);
		quad.tr = Point2f(x2, mean(right_corners));
		quad.br = Point2f(mean(bottom_corners), y2);
		quad.bl = Point2f(x1, mean(left_corners));
	}
}

void drawtest(Quad& q,Mat& image)
{
	circle(image, q.bl, 2, Scalar(0, 255, 0), -1, 8, 0);
	circle(image, q.br, 2, Scalar(0, 255, 0), -1, 8, 0);
	circle(image, q.tl, 2, Scalar(0, 255, 0), -1, 8, 0);
	circle(image, q.tr, 2, Scalar(0, 255, 0), -1, 8, 0);

}
//真正确定四个角点了
Mat PerspectiveTrans::transform(Mat image)
{
	//src的写法不是写绝对位置是相对位置
	Quad corners = determine_relative_location();
	//drawtest(bottom_quad,image);

	vector<Point2f> src{corners.tl,corners.tr,corners.br,corners.bl};
	vector<Point2f> dst{ Point2f(0,0),Point2f(150,0),Point2f(150,150),Point2f(0,150)};

	Mat warp_matrix = getPerspectiveTransform(src, dst);
	Mat QR_image (150, 150, CV_8UC3);
	warpPerspective(image, QR_image, warp_matrix,Size(150,150));
	return QR_image;

}
Point2f PerspectiveTrans::find_4th()
{
	Point2f target;

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
	
	return target;
}

Quad PerspectiveTrans::determine_absolution_location()
{
	Point2f fourth = find_4th();
	Quad corners;
	switch (ORIENTATION)
	{
	case NorthWest:
		corners = { top_quad.tl,right_quad.tr,fourth,bottom_quad.bl };
		break;
	case SouthEast:
		corners = { fourth,bottom_quad.tr,top_quad.br,right_quad.bl };
		break;
	case SouthWest:
		corners = { right_quad.tl,fourth,bottom_quad.br,top_quad.bl };
		break;
	case NorthEast:
		corners = { bottom_quad.tl,top_quad.tr,right_quad.br,fourth };
		break;
	default:
		break;
	}
	return corners;
}


Quad PerspectiveTrans::determine_relative_location()
{
	Point2f fourth = find_4th();
	Quad relative_corners;
	switch (ORIENTATION)
	{
	case NorthWest:
		relative_corners = { top_quad.tl,right_quad.tr,fourth,bottom_quad.bl };
		break;
	case SouthEast:
		relative_corners = { top_quad.br,right_quad.bl,fourth,bottom_quad.tr };
		break;
	case SouthWest:
		relative_corners = { top_quad.bl,right_quad.tl,fourth,bottom_quad.br };
		break;
	case NorthEast:
		relative_corners = { top_quad.tr,right_quad.br,fourth,bottom_quad.tl };
		break;
	default:
		break;
	}
	return relative_corners;
}
Point2f PerspectiveTrans::get_intersection_point(Point2f a1, Point2f& a2, Point2f& b1, Point2f& b2)
{

	Point2f r(a2 - a1);
	Point2f s(b2 - b1);
	Point2f t(b1 - a1);
	double ratio = cross(t, s) / cross(r, s);
	Point2f intersection = ratio * r + a1;
	return intersection;
}