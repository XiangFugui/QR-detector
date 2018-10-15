#pragma once

#include"opencv2/opencv.hpp"
#include<iostream>
//#include<string>
//#include<vector>
//#include"pattern_finding.h"
#include "mytool.h"
using namespace std;
using namespace cv;

typedef vector<Point> Contour;
typedef vector<Vec4i> Hierarchy;
//extern class PatternFinding detector;
class ImageHandler
{
	friend void show_rect(int contour_id, cv::Mat& image, vector<Contour>& contours);
	friend void show_contour(int contour_id, cv::Mat& image, vector<Contour>& contours);
public:
	ImageHandler(string path);
	~ImageHandler();
	vector<Contour> get_patterns();
	void show_image();
	void convert2black_and_white();
	void QR_in_image();
private:
	Mat src_image;
	Mat threshold_image;
	vector<Contour> QR_patterns;
};

