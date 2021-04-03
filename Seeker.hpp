#pragma once
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;
// using namespace sciplot;
class Ball;
class Seeker
{
public:

	Seeker(int window_size);

	static const int MIN_OBJECT_AREA;
	static const int MAX_OBJECT_AREA;
	static int num_of_chart_data;

	cv::Mat frame;
	vector<Point> contour;

	static void vectorRotateLeft(vector<double>* ydata, int last_largest_num); // В дальнейшем избавиться 
	bool findObject(cv::Mat* Output_frame, cv::Mat* BGR_frame);
	void drawObject(Mat *BGR_frame, float xPos, float yPos, float zPos, int xPosPx, int yPosPx);

	vector<double> getYData(); // В дальнейшем избавиться 
	vector<double> getXData(); // В дальнейшем избавиться 

	int get_pxX();
	int get_pxY();
	int get_pxDiameter();
	
private:
	int pxX;
	int pxY;
	float pxDiameter;
	int datasize;
	vector<double> ydata;
	vector<double> xdata;
	vector<double>* pointer_ydata;
	vector<double>* pointer_xdata;
};

