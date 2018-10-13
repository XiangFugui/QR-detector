#pragma once
#include"image_handler.h"
#include "pattern_finding.h"
extern class Pattern_finding detector;

void show_rect(int contour_id, cv::Mat& image, Pattern_finding& detector);

void show_contour(int contour_id, cv::Mat& image, Pattern_finding& detector);
