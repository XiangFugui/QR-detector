#pragma once

#include"opencv2/opencv.hpp"
#include<iostream>
//#include<string>
//#include<vector>
#include"pattern_finding.h"
#include "mytool.h"
using namespace std;
using namespace cv;

typedef vector<Point> Contour;
typedef vector<Vec4i> Hierarchy;
extern class Pattern_finding detector;
class Image_handler
{
	friend void show_rect(int contour_id, cv::Mat& image, Pattern_finding& detector);
	friend void show_contour(int contour_id, cv::Mat& image, Pattern_finding& detector);
public:
	Image_handler(string path);
	~Image_handler();
	vector<int> get_patterns();
	void show_image();
	void convert2black_and_white();

private:
	Mat src_image;
	Mat threshold_image;
};

