#pragma once
#include"opencv2/opencv.hpp"
#include<iostream>
#include<vector>
using namespace std;
using namespace cv;

typedef vector<Point> Contour;
typedef vector<Vec4i> Hierarchy;

class PatternFinding
{
public:
	PatternFinding(Mat& image);
	~PatternFinding();
	vector<Contour>& detect(int layer = 6);

private:
	bool check_area_of_contour(const int index);
	bool check_contour(const int, int layer);
	bool is_possible_index(const int index);
	void mark(const int index);
	int get_grandson(const int index);
	int get_grandparent(const int index);
	vector<Contour> contours;
	Hierarchy hierarchy;
	vector<Contour> QR_patterns;
	unsigned int *marked_hashtable;
};
