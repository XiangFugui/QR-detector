#pragma once
#include"image_handler.h"
#include "pattern_finding.h"
extern class vector<Contour> contours;


void show_rect(int contour_id, cv::Mat& image, vector<Contour>& contours);

void show_contour(int contour_id, cv::Mat& image, vector<Contour>& contours);
