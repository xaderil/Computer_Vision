#pragma once
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

class Ball
{
public:
	
	Ball(string Color);

	int getXPos();
	void setXPos(int x);

	int getYPos();
	void setYPos(int y);

	Scalar getHSVmin();
	Scalar getHSVmax();

	String getColor() {return Color;}

	void setHSVmin(Scalar min);
	void setHSVmax(Scalar max);

private:
	int xPos, yPos;
	string Color;
	Scalar HSVmin, HSVmax;
};

