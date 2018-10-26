#include "stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include"pattern_finding.h"
#include<ctime>
using namespace std;
using namespace cv;

//优化idea:管你成功不成功都把你mark了，成功的我check_area_of_contour我传入最内层的index
bool PatternFinding::check_contour(const int index, int possible_layer = 6)
{
	int total_layer = 1;
	int current_child = hierarchy.at(index)[2];
	int next_child= current_child;
	while (next_child != -1)
	{
		current_child = next_child;
		mark(current_child);
		total_layer++;
		next_child= hierarchy.at(current_child)[2];
	}
	//cout << "total_layer" << total_layer<<endl;

	if (total_layer >= possible_layer)
	{
		return (check_area_of_contour(current_child));
	}
	return false;
}

bool PatternFinding::is_possible_index(const int index)
{
	if (marked_hashtable[index]==1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void PatternFinding::mark(const int index)
{
	marked_hashtable[index] = 1;
}



vector<Contour>& PatternFinding::detect(int possible_layer)
{
	clock_t t1,t2;
	t1 = clock();
	int num_contours = contours.size();
	marked_hashtable = new unsigned int[num_contours]();

	for (int index = 0; index<num_contours; index++)
	{
		if (is_possible_index(index) == true)
		{
			if (check_contour(index, possible_layer))
			{
				QR_patterns.push_back(contours[index]);
			}
		}
		else
		{
			continue;
		}
		
	}
	delete[] marked_hashtable;
	marked_hashtable = NULL;
	cout << "posible Contours " << QR_patterns.size() << endl;
	t2 = clock();
	cout << "t2-t1" << t2 - t1 << endl;
	return QR_patterns;
}

int PatternFinding::get_grandson(const int index)
{
	return hierarchy.at(hierarchy.at(index)[2])[2];
}

int PatternFinding::get_grandparent(const int index)
{
	return hierarchy.at(hierarchy.at(index)[3])[3];
}


bool PatternFinding::check_area_of_contour(const int innermost_layer)
{
	cout << "check area" << endl;
	int second_layer_index = get_grandparent(innermost_layer);
	int outermost_layer_index = get_grandparent(second_layer_index);

	double outermost_layer_area = contourArea(contours.at(outermost_layer_index));
	double second_layer_area = contourArea(contours.at(second_layer_index));
	double innermost_layer_area = contourArea(contours.at(innermost_layer));

	double ratio12 = outermost_layer_area / (second_layer_area + 1e-5);
	double ratio23 = second_layer_area / (innermost_layer_area + 1e-5);

	cout << outermost_layer_area << endl;
	cout<< second_layer_area <<endl;
	cout<< innermost_layer_area <<endl;
	
	cout<<"" <<endl;
	cout<< ratio12 <<endl;
	cout<< ratio23 <<endl;
	cout << "" << endl;


	//ratio12 应该是1.97 ratio23 2.78
	//然而调试的过程中我不得不加大允许的范围
	return (ratio12 < 3 && ratio23 < 4);
}



PatternFinding::PatternFinding(Mat& image)
{
	findContours(image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	cout << "hierarchy size" << contours.size() << endl;
}


PatternFinding::~PatternFinding()
{
	delete[] marked_hashtable;
}
