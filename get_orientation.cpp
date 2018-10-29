#pragma once
#include "stdafx.h"
#include<vector>
#include<iostream>
#include<map>
#include"opencv2/opencv.hpp"
#include"get_orientation.h"
//#include "mytool.h"
using namespace std;
using namespace cv;

//typedef pair<Point2f, int> pi;
//extern double calc_slope(const Point2f& pointA, const Point2f& pointB);
//extern double calc_perpendicular_dist(const Point2f& pointA, const Point2f& pointB, const Point2f& pointC, double slope);

enum orientation { NorthWest, SouthEast, SouthWest, NorthEast };
orientation ORIENTATION;

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

	piA = { point_A,0 };
	piB = { point_B,1 };
	piC = { point_C,2 };

}


Position& GetOrientation::find_orientation()
{
	double dist_AB = calc_dist(piA.first, piB.first);
	double dist_AC = calc_dist(piA.first, piC.first);
	double dist_BC = calc_dist(piB.first, piC.first);


	double longest = max(dist_AB, max(dist_AC, dist_BC));

	if (dist_AB==longest)
	{
		get_relation(piA, piB, piC);
	}
	else if (dist_AC==longest)
	{
		get_relation(piA, piC, piB);
	}
	else if (dist_BC==longest)
	{
		get_relation(piB, piC, piA);
	}
	//else
	//{
	//	return;//这里我不知道怎么写
	//}
	return position;
}


bool GetOrientation::get_relation(const pi& median1, const pi& median2, const pi& outlier)
{
	double slope = calc_slope(median1.first, median2.first);
	double ppd_dist = calc_perpendicular_dist(median1.first, median2.first, outlier.first, slope);

	cout << "slope" << slope << endl;
	cout << "dist" << ppd_dist << endl;
	if (ppd_dist == 0)
	{
		//TO DO
		cerr << "poisition 在一条支线上" << endl;
		return false;
	}
	//那这么看来，我的这个思路是完全可以理清3个位置的
	if (slope < 0)
	{
		//回    回
		//
		//回
		if (ppd_dist < 0)
		{
			if (median1.first.y > median2.first.y)
			{
				position.Bottom_mass_point = median1.first;
				position.Right_mass_point = median2.first;
			}
			else
			{
				position.Bottom_mass_point = median2.first;
				position.Right_mass_point = median1.first;
			}
			ORIENTATION = NorthWest;
		}
		//     回
		//
		//回	   回
		else
		{
			if (median1.first.y > median2.first.y)
			{
				position.Right_mass_point = median1.first;
				position.Bottom_mass_point = median2.first;
			}
			else
			{
				position.Right_mass_point = median2.first;
				position.Bottom_mass_point = median1.first;
			}
			ORIENTATION = SouthEast;

		}

	}
	else
	{
		//回
		//
		//回   回
		if (ppd_dist >= 0)
		{
			if (median1.first.x > median2.first.x)
			{
				position.Bottom_mass_point = median1.first;
				position.Right_mass_point = median2.first;
			}
			else
			{
				position.Bottom_mass_point = median2.first;
				position.Right_mass_point = median1.first;
			}
			ORIENTATION = SouthWest;
		}

		else
		{
			//回  回
			//
			//    回
			if (median1.first.x > median2.first.x)
			{
				position.Bottom_mass_point = median2.first;
				position.Right_mass_point = median1.first;
			}
			else
			{
				position.Bottom_mass_point = median1.first;
				position.Right_mass_point = median2.first;
			}
			ORIENTATION = NorthEast;
		}
	}
	if (position.Bottom_mass_point == median1.first)
	{
		position.Bottom = QR_patterns[median1.second];
		position.Right = QR_patterns[median2.second];
	}
	else
	{
		position.Bottom = QR_patterns[median2.second];
		position.Right = QR_patterns[median1.second];
	}
	position.Top_mass_point = outlier.first;
	position.Top = QR_patterns[outlier.second];
	return true;
}


GetOrientation::~GetOrientation()
{

}