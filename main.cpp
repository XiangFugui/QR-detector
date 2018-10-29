#include "stdafx.h"
#include <iostream>
#include<string>
#include<vector>
#include<time.h>
#include"opencv2/opencv.hpp"
#include"image_handler.h"
#include"video_handler.h"

using namespace std;
using namespace cv;


int main()
{
	VideoHandler vhdl;
	vhdl.setInput("image/4second.mp4");
	vhdl.run();

	waitKey(0);
}



