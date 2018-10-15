#pragma once
#include"opencv2/opencv.hpp"
using namespace std;
using namespace cv;

typedef vector<Point> Contour;

struct Position
{
	Point Top;
	Point Bottom;
	Point Right;
};

class Determine_orientation
{
public:
	Determine_orientation(Contour& contourA, Contour& contourB, Contour& contourC);
	~Determine_orientation();
	Position& find_orientation();
private:
	static double calc_slope(const Point& pointA, const Point& pointB);
	static double calc_perpendicular_dist(const Point& pointA, const Point& pointB, const Point& pointC, double slope);
	static double calc_dist(const Point& pointA, const Point& pointB);
	void get_points_relation(const Point& median1, const Point& median2, const Point& outlier);
	Point point_A, point_B, point_C;
	Position position;
};


