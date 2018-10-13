#pragma once
#include"opencv2/opencv.hpp"
#include<iostream>
#include<vector>
#include "mytool.h"

using namespace std;
using namespace cv;

typedef vector<Point> Contour;
typedef vector<Vec4i> Hierarchy;


class Pattern_finding
{
	friend void show_rect(int contour_id, Mat& image, Pattern_finding& detector);
	friend void show_contour(int contour_id, cv::Mat& image, Pattern_finding& detector);
public:
	Pattern_finding(Mat& image);
	~Pattern_finding();
	vector<int> detect(int layer=6);

private:
	//我都想友元handler里的hierarchy和contours了
	bool check_area_of_contour(const int index);
	bool check_contour(const int index,int layer);
	bool is_possible_index(const int index);
	void mark(const int index);
	int get_grandson(const int index);
	vector<Contour> contours;
	Hierarchy hierarchy;
	int *marked_hashtable;
};


