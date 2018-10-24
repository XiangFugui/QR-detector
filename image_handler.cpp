#include "stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include<string>
#include"image_handler.h"

using namespace std;
using namespace cv;

typedef vector<Point> Contour;
typedef vector<Vec4i> Hierarchy;

ImageHandler::ImageHandler(string path)//
{
	src_image = imread(path, IMREAD_COLOR);
	if (!src_image.data)
	{
		cerr << "��ȡͼƬ����" << endl;
	}
}

ImageHandler::ImageHandler(Mat image)
{
	src_image = image;
}

ImageHandler::~ImageHandler()
{

}

void ImageHandler::convert2black_and_white()
{
	cvtColor(src_image, threshold_image, CV_BGR2GRAY);
	//threshold(threshold_image, threshold_image, 120, 255, CV_THRESH_BINARY);

	double threshold_value=threshold(threshold_image, threshold_image, 0, 255, CV_THRESH_OTSU);
	//�����з�����Щ���������������˲��󷴶�����.
	//��ЩͼƬ���۹۲���������ʶ�𲻳�������kernel��(5,5)->(3,3)�ͺ���
	GaussianBlur(threshold_image, threshold_image, Size(3, 3), 0);//��Ҫ��
	Canny(threshold_image, threshold_image, 100, 200);
}

vector<Contour> ImageHandler::get_patterns()
{

	PatternFinding detector(threshold_image);
	QR_patterns = detector.detect();

	//show_contours(src_image, QR_patterns);
	
	return QR_patterns;
}

Position& ImageHandler::QR_in_image()
{
	GetOrientation orientation_obj(QR_patterns);
	position = orientation_obj.find_orientation();
	return position;
}

void ImageHandler::affine_transform()
{
	AffineTrans trans_obj(position);
	QR_image =trans_obj.transform(src_image);
	imshow("QR_trans", QR_image);
}
void ImageHandler::persepective_transform()
{
	PerspectiveTrans psp(position);
	QR_image = psp.transform(src_image);
	imshow("QR_trans", QR_image);
	
}