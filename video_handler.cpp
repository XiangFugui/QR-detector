#include "stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include"video_handler.h"
using namespace std;
using namespace cv;
void VideoHandler::run()
{
	Mat frame;
	Mat output;

	if (!isOpened())
		return;
	capture.read(frame);
	setReadRate();
	while (!isStopped())
	{
		ImageHandler ihdl(frame);
		if (ihdl.run())
		{
			stop = true;
			cout << "识别到了" << endl;
			displayOutput(ihdl);
			break;
		}
		if (!jump(frame))
		{
			break;
		}
	}
	//if (stop)
	//{
	//	displayOutput(ihdl);

	//}
}

bool VideoHandler::jump(Mat& frame)
{
	fnumber = fnumber + readRate;
	if (jumpTo(fnumber - 1))
	{
		capture.read(frame);
		cout << "/n" << endl;
		printf("切换至下一帧\n");
		return true;
	}
	return false;
}

bool VideoHandler::jumpTo(double position)
{
	if (position>capture.get(CV_CAP_PROP_FRAME_COUNT))
	{
		return false;
	}
	capture.set(CAP_PROP_POS_FRAMES, position);
	return true;
}

int VideoHandler::setReadRate()
{
	return (setReadRate(0.3));
}

int VideoHandler::setReadRate(double ratio)
{
	double frameRate = capture.get(CV_CAP_PROP_FPS);
	readRate = frameRate * ratio;
	return readRate;
}


int VideoHandler::setDelay()
{
	double rate = capture.get(CV_CAP_PROP_FPS);
	delay = static_cast<int>(1000 / rate);
	return delay;

}

int VideoHandler::setDelay(int m)
{
	delay = m;
	return delay;
}

bool VideoHandler::isOpened()
{
	return capture.isOpened();
}

bool VideoHandler::isStopped()
{
	return stop;
}

bool VideoHandler::setInput(string path)
{
	return capture.open(path);
}

void VideoHandler::displayInput(ImageHandler& ihdl, string wn)
{
	imshow(wn,ihdl.src_image);
}

void VideoHandler::displayOutput( ImageHandler& ihdl, string wn)
{
	cout <<"debug" <<ihdl.QR_image.cols << endl;
	imshow(wn,ihdl.QR_image);
}

VideoHandler::VideoHandler() :fnumber(0), stop(false), readRate(0)
{
}

VideoHandler::~VideoHandler()
{
	capture.release();
}