#pragma once
#include"opencv2/opencv.hpp"
#include<iostream>
#include<string>
#include<vector>
#include"image_handler.h"

using namespace std;
using namespace cv;

class VideoHandler
{
public:
	VideoHandler();
	~VideoHandler();
	bool setInput(string);
	bool jumpTo(double position);
	void run();
	int setReadRate();
	int setReadRate(double ratio);
	void displayInput(ImageHandler&, string wn = "input" );
	void displayOutput(ImageHandler&, string wn = "output");
private:
	VideoCapture capture;
	void(*process)(Mat& in, Mat& out);
	int setDelay();
	int setDelay(int step);
	bool isOpened();
	bool isStopped();
	bool jump(Mat& frame);
	
	int readRate;
	//µ±Ç°Ö¡Êý
	int fnumber;
	int delay;
	bool stop;
};
