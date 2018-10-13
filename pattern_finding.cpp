#include "stdafx.h"
#include"opencv2/opencv.hpp"
#include<iostream>
#include"pattern_finding.h"
using namespace std;
using namespace cv;

bool Pattern_finding::check_contour(const int index, int possible_layer = 6)
{
	int total_layer = 1;
	int child_index = hierarchy.at(index)[2];
	vector<int> branch = { index };

	while (child_index != -1)
	{
		branch.push_back(child_index);
		total_layer++;
		child_index = hierarchy.at(child_index)[2];
	}
	//cout << "total_layer" << total_layer<<endl;

	//父轮廓不行，子怎么会行，标记他们。怎么感觉不太可靠
	if (total_layer < possible_layer)
	{
		for (int var : branch)
		{
			mark(var);
		}
		return false;
	}

	return (check_area_of_contour(index));

}

bool Pattern_finding::is_possible_index(const int index)
{
	return !(marked_hashtable[index]);
}

void Pattern_finding::mark(const int index)
{
	marked_hashtable[index] = 1;
}



vector<int> Pattern_finding::detect(int possible_layer)
{
	vector<int> pattern_index;
	int num_contours = contours.size();
	marked_hashtable = new int[num_contours]();
	for (int index = 0; index<num_contours; index++)
	{
		if (is_possible_index(index) == false)
		{
			continue;
		}
		
		if (check_contour(index, possible_layer))
		{
			pattern_index.push_back(index);
		}
	}
	return pattern_index;
}

int Pattern_finding::get_grandson(const int index)
{
	return hierarchy.at(hierarchy.at(index)[2])[2];
}

bool Pattern_finding::check_area_of_contour(const int first_layer_index)
{
	int second_layer_index = get_grandson(first_layer_index);
	int third_layer_index = get_grandson(second_layer_index);

	double first_layer_area = contourArea(contours.at(first_layer_index));
	double second_layer_area = contourArea(contours.at(second_layer_index));
	double third_layer_area = contourArea(contours.at(third_layer_index));

	double ratio12 = first_layer_area / (second_layer_area + 1e-5);
	double ratio23 = second_layer_area / (third_layer_area + 1e-5);

	cout << first_layer_area << endl;
	cout<< second_layer_area <<endl;
	cout<< third_layer_area <<endl;
	
	cout<<"" <<endl;
	cout<< ratio12 <<endl;
	cout<< ratio23 <<endl;
	cout << "" << endl;


	//ratio12 应该是1.97 ratio23 2.78
	//然而调试的过程中我不得不加大允许的范围
	return (ratio12 < 3 && ratio23 < 4);
}



Pattern_finding::Pattern_finding(Mat& image)
{

	findContours(image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	//cout << "hierarchy size" << contours.size() << endl;
}


Pattern_finding::~Pattern_finding()
{
	delete[] marked_hashtable;
}
