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

	Seeker();

	static const int MIN_OBJECT_AREA;
	static const int MAX_OBJECT_AREA;

	cv::Mat frame;
	vector<Point> contour;

	bool findObject(cv::Mat* Output_frame, cv::Mat* BGR_frame);
	void drawObject(Mat *BGR_frame, float xPos, float yPos, float zPos, int xPosPx, int yPosPx);

	// vector<double> getYData(); // В дальнейшем избавиться 
	// vector<double> getXData(); // В дальнейшем избавиться 

	int get_pxX();
	int get_pxY();
	int get_pxDiameter();
	
private:
	int pxX;
	int pxY;
	float pxDiameter;

};

