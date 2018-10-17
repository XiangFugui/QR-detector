#pragma once
#include"opencv2/opencv.hpp"
using namespace std;
using namespace cv;

typedef vector<Point> Contour;
//这些点都是质心
struct Position
{
	Point2f Top;
	Point2f Bottom;
	Point2f Right;
};

class Determine_orientation
{
public:
	Determine_orientation(Contour& contourA, Contour& contourB, Contour& contourC);
	~Determine_orientation();
	Position& find_orientation();
private:
	double calc_slope(const Point2f& pointA, const Point2f& pointB);
	double calc_perpendicular_dist(const Point2f& pointA, const Point2f& pointB, const Point2f& pointC, double slope);
	double calc_dist(const Point2f& pointA, const Point2f& pointB);
	void get_points_relation(const Point2f& median1, const Point2f& median2, const Point2f& outlier);
	Point2f point_A, point_B, point_C;
	Position position;
};


