#include "stdafx.h"
#include <iostream>
#include"opencv2/opencv.hpp"
#include"image_handler.h"
#include<string>
#include<vector>
using namespace std;
using namespace cv;


int main()
{
	ImageHandler hdl("image/timg1.jpg");
	hdl.convert2black_and_white();
	hdl.get_patterns();
	hdl.show_corners();
	hdl.show_rect();
	hdl.show_contour_point();
	waitKey(0);
    return 0;
}



