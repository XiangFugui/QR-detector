#pragma once
#include"opencv2/opencv.hpp"
#include<iostream>
#include<string>
#include<vector>
#include"pattern_finding.h"
#include "get_orientation.h"
#include"transform.h"
using namespace std;
using namespace cv;

typedef vector<Point> Contour;
typedef vector<Vec4i> Hierarchy;


class ImageHandler
{
	//friend void m_show_rect(int contour_id, cv::Mat& image, vector<Contour>& contours);
	//friend void m_show_contour(int contour_id, cv::Mat& image, vector<Contour>& contours);
	//friend void m_show_contours(cv::Mat& image, vector<Contour>& contours);
	friend class VideoHandler;
public:
	ImageHandler(string path);
	ImageHandler(Mat image);
	~ImageHandler();
	bool run();
	bool get_patterns();
	bool convert2black_and_white();
	Position& figure_orientation();
	Mat affine_transform();
	Mat persepective_transform();
	
	
	void show_output();
	void show_position(Position& );
	void show_contours(Mat image);
	void show_contours();
	void show_only_contours();
	void show_contour_point(int flag = 1);
	void show_corner(int id);
	void show_corners();
	void show_thre_image();
	void show_src_image();
	void show_rect(int contour_id =0);
private:
	Mat src_image;
	Mat threshold_image;
	Mat QR_image;
	Mat trans_image;
	vector<Contour> QR_patterns;
	Position position;
};

