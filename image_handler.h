#pragma once

#include"opencv2/opencv.hpp"
#include<iostream>
//#include<string>
//#include<vector>
//#include"pattern_finding.h"
#include "mytool.h"
#include "determine_orientation.h"
using namespace std;
using namespace cv;

typedef vector<Point> Contour;
typedef vector<Vec4i> Hierarchy;
//extern class PatternFinding detector;
class ImageHandler
{
	friend void m_show_rect(int contour_id, cv::Mat& image, vector<Contour>& contours);
	friend void m_show_contour(int contour_id, cv::Mat& image, vector<Contour>& contours);
	friend void m_show_contours(cv::Mat& image, vector<Contour>& contours);
public:
	ImageHandler(string path);
	~ImageHandler();
	vector<Contour> get_patterns();
	void show_image();
	void convert2black_and_white();
	Position& QR_in_image();
	Mat& transform();
	void show_position();
	void show_contours();
	void show_center();
	void show_corner(int id);
	void show_corners();
private:
	Mat src_image;
	Mat threshold_image;
	Mat QR_image;
	vector<Contour> QR_patterns;
	Position position;
};

