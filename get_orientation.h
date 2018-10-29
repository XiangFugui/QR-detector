#pragma once
#include"opencv2/opencv.hpp"
#include<vector>
#include"mytool.h"
using namespace std;
using namespace cv;

typedef vector<Point> Contour;
typedef pair<Point2f, int> pi;


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
	bool get_relation(const pi& median1, const pi& median2, const pi& outlier);
	void get_figuration();
	Point2f point_A, point_B, point_C;
	vector<Contour> QR_patterns;
	Position position;
	pi piA;
	pi piB;
	pi piC;
};


