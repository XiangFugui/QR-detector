#pragma once
#include "stdafx.h"
#include<vector>
#include<iostream>
#include<map>
#include"opencv2/opencv.hpp"
#include"get_orientation.h"
using namespace std;
using namespace cv;
GetOrientation::GetOrientation(vector<Contour>& QR_patterns)
{
	this->QR_patterns = QR_patterns;

	Moments momentA = moments(QR_patterns[0]);
	Moments momentB = moments(QR_patterns[1]);
	Moments momentC = moments(QR_patterns[2]);
	point_A.x = (momentA.m10 / momentA.m00);
	point_A.y = (momentA.m01 / momentA.m00);

	point_B.x = (momentB.m10 / momentB.m00);
	point_B.y = (momentB.m01 / momentB.m00);

	point_C.x = (momentC.m10 / momentC.m00);
	point_C.y = (momentC.m01 / momentC.m00);
	cntInd_of_mess = { { point_A ,0},{ point_B,1},{ point_C,2 } };

}


Position& GetOrientation::find_orientation()
{
	double dist_AB = calc_dist(point_A, point_B);
	double dist_AC = calc_dist(point_A, point_C);
	double dist_BC = calc_dist(point_B, point_C);


	double longest = max(dist_AB, max(dist_AC, dist_BC));

	if (dist_AB==longest)
	{
		get_relation(point_A, point_B, point_C);
	}
	if (dist_AC==longest)
	{
		get_relation(point_A, point_C, point_B);
	}
	if (dist_BC==longest)
	{
		get_relation(point_B, point_C, point_A);
	}

	return position;
}


void GetOrientation::get_relation(const Point2f& median1, const Point2f& median2, const Point2f& outlier)
{
	//这两个函数有问题
	double slope = calc_slope(median1, median2);
	double ppd_dist = calc_perpendicular_dist(median1, median2, outlier, slope);

	cout << "slope" << slope<< endl;
	cout << "dist" << ppd_dist<<endl;
	if (ppd_dist == 0)
	{
		//TO DO
		cerr << "poisition 在一条支线上" << endl;
		exit(0);
	}
	//那这么看来，我的这个思路是完全可以理清3个位置的
	if (slope < 0 )       
	{
		//回    回
		//
		//回
		if (ppd_dist < 0)
		{
			if (median1.y > median2.y)
			{
				position.Bottom_mass_point = median1;
				position.Right_mass_point = median2;
			}
			else
			{
				position.Bottom_mass_point = median2;
				position.Right_mass_point = median1;

			}
			ORIENTATION = NorthWest;
		}
		//     回
		//
		//回	   回
		else
		{
			if (median1.y > median2.y)
			{
				position.Right_mass_point = median1;
				position.Bottom_mass_point = median2;
			}
			else
			{
				position.Right_mass_point = median2;
				position.Bottom_mass_point = median1;
			}
		}
		ORIENTATION = SouthEast;

	}
	else
	{
		//回
		//
		//回   回
		if (ppd_dist >= 0)
		{
			if (median1.x > median2.x)
			{
				position.Bottom_mass_point = median1;
				position.Right_mass_point = median2;
			}
			else
			{
				position.Bottom_mass_point = median2;
				position.Right_mass_point = median1;
			}
			ORIENTATION = SouthWest;
		}

		else
		{
		//回  回
		//
		//    回
			if (median1.x > median2.x)
			{
				position.Bottom_mass_point = median2;
				position.Right_mass_point = median1;
			}
			else
			{
				position.Bottom_mass_point = median1;
				position.Right_mass_point = median2;
			}
			ORIENTATION = NorthWest;

		}
	}
	position.Top_mass_point = outlier;
	get_figuration();
}

void GetOrientation::get_figuration()
{
	//太花式了。。。
	//这种方法太智息了
	//太智熄了

	position.Top = QR_patterns[cntInd_of_mess[position.Top_mass_point]];
	position.Right = QR_patterns[cntInd_of_mess[position.Right_mass_point]];
	position.Bottom = QR_patterns[cntInd_of_mess[position.Bottom_mass_point]];
}

double GetOrientation::calc_slope(const Point2f& pointA, const Point2f& pointB)
{
	double temp = pointB.x - pointA.x;
	if (temp == 0)
		return temp;
	double result = (pointB.y - pointA.y) /temp;
	return result;
}


double GetOrientation::calc_perpendicular_dist(const Point2f& pointA, const Point2f& pointB, const Point2f& pointC, double slope)
{
	double a, b, c, dist;
	a = slope * (-1);
	b = 1;
	c = (slope*pointB.x) - pointB.y;
	dist = (a*pointC.x + b * pointC.y + c) / sqrt((a*a) + (b*b));
	return dist;
}

double GetOrientation::calc_dist(const Point2f& pointA, const Point2f& pointB)
{
	return sqrt(pow(pointA.x - pointB.x, 2) + pow(pointA.y - pointB.y, 2));
}

GetOrientation::~GetOrientation()
{

}