#pragma once
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <iostream>
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
	static vector<Point> circle_contour;
	

	cv::Mat frame;
	vector<Point> contour;
	Point2f vtx[4];

	bool findObject(cv::Mat* Output_frame, cv::Mat* BGR_frame, int FRAME_WIDTH, int FRAME_HEIGHT);
	void drawObject(Mat *BGR_frame, float xPos, float yPos, float zPos, int xPosPx, int yPosPx);
	bool checkContourBorder(int FRAME_WIDTH, int FRAME_HEIGHT);
	double compareWithCircle();
	int get_pxX();
	int get_pxY();
	int get_pxDiameter();
	
private:
	int pxX;
	int pxY;
	float pxDiameter;

};

