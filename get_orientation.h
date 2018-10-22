#pragma once
#include"opencv2/opencv.hpp"
#include<vector>
using namespace std;
using namespace cv;

typedef vector<Point> Contour;

enum orientation { NorthWest, SouthEast, SouthWest, NorthEast };
orientation ORIENTATION;

//这些点都是质心
struct Position
{
	Point2f Top_mass_point;
	Point2f Bottom_mass_point;
	Point2f Right_mass_point;
	Contour Top;
	Contour Bottom;
	Contour Right;

};

class GetOrientation
{
public:
	GetOrientation(vector<Contour>& QR_patterns);
	~GetOrientation();
	Position& find_orientation();
private:
	double calc_slope(const Point2f& pointA, const Point2f& pointB);
	double calc_perpendicular_dist(const Point2f& pointA, const Point2f& pointB, const Point2f& pointC, double slope);
	double calc_dist(const Point2f& pointA, const Point2f& pointB);
	void get_relation(const Point2f& median1, const Point2f& median2, const Point2f& outlier);
	void get_figuration();
	Point2f point_A, point_B, point_C;
	vector<Contour> QR_patterns;
	map<Point2f, int> cntInd_of_mess;
	Position position;
};


