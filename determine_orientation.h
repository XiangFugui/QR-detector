#pragma once
#include"opencv2/opencv.hpp"
//#include<>
using namespace std;
using namespace cv;

typedef vector<Point> Contour;

class Determine_orientation
{
public:
	Determine_orientation(Contour& contourA, Contour& contourB, Contour& contourC);
	~Determine_orientation();
	void find_orientation();
private:
	double calc_slope(const Point& pointA, const Point& pointB);
	double calc_perpendicular_dist(const Point& pointA, const Point& pointB, const Point& pointC, double slope);
	double calc_dist(const Point& pointA, const Point& pointB);
	Point point_A, point_B, point_C;

};

Determine_orientation::Determine_orientation(Contour& contourA, Contour& contourB, Contour& contourC)
{
	Moments momentA = moments(contourA);
	Moments momentB = moments(contourB);
	Moments momentC = moments(contourC);
	point_A.x = (int)(momentA.m10 / momentA.m00);
	point_A.y = (int)(momentA.m01 / momentA.m00);

	point_B.x = (int)(momentB.m10 / momentB.m00);
	point_B.y = (int)(momentB.m01 / momentB.m00);

	point_C.x = (int)(momentC.m10 / momentC.m00);
	point_C.y = (int)(momentC.m01 / momentC.m00);

}
void Determine_orientation::find_orientation()
{
	double slope_AB = calc_slope(point_A, point_B);
	double slope_AC = calc_slope(point_A, point_C);
	double slope_BC = calc_slope(point_B, point_C);

	double dist_AB = calc_dist(point_A, point_B);
	double dist_AC = calc_dist(point_A, point_C);
	double dist_BC = calc_dist(point_B, point_C);
		

}
double Determine_orientation::calc_slope(const Point& pointA, const Point& pointB)
{
	return (pointB.y - pointA.x) / (pointB.x - pointA.y);
}

double Determine_orientation::calc_perpendicular_dist(const Point& pointA, const Point& pointB, const Point& pointC,double slope)
{
	double a, b, c, dist;
	a = slope * (-1);
	b = 1;
	c = (slope*pointB.x) - pointB.y;
	dist = (a*pointC.x + b * pointC.y + c) / sqrt((a*a) / (b*b));
	return dist;
}

double Determine_orientation::calc_dist(const Point& pointA, const Point& pointB)
{
	return sqrt(pow(pointA.x-pointB.x,2)+pow(pointA.y - pointB.y, 2))
}
Determine_orientation::~Determine_orientation()
{

}