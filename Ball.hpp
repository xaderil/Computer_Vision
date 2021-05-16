#pragma once
#include <opencv2/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class Ball
{
public:
	
	Ball(string Color);


	static int    f;	// Коэффициент фокусного расстояния
	const float   d = 0.04; // Диаметр мячика

	int           getXPos();
	void 		  setXPos(int x);
	void          setXRealPos(int x);
	void          calculateXRealPos(int Frame_Width, int Frame_Height);
	float         getXRealPos();

	int           getYPos();
	void          setYPos(int y);
	void          setYRealPos(int y);
	void          calculateYRealPos(int Frame_Width, int Frame_Height);
	float         getYRealPos();

	int           getZDiameter();
	void          setZDiameter(int z);
	void          setZRealPos(int z);
	void          calculateZRealPos();
	float         getZRealPos();

	Scalar        getHSVmin();
	Scalar        getHSVmax();

	String        getColor() {return Color;}

	void          setHSVmin(Scalar min);
	void          setHSVmax(Scalar max);

	vector <double> staticXData;
	vector <double> staticYData;
	vector <double> staticZData;

private:

	float         	xPos, yPos, zDiameter, XRealPos, YRealPos, ZRealPos;
	string        	Color;
	Scalar        	HSVmin, HSVmax;

};

