#pragma once
#include <opencv2/core.hpp>
#include <iostream>
#include "JSON_Worker.hpp"

using namespace std;
using namespace cv;
using namespace nlohmann;

typedef vector<double> VecD;

class JSON;

class Ball
{
public:
	
	Ball(string Color);

	static int    f;	// Коэффициент фокусного расстояния
	static int 	  H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX;

	const  float  d = 0.04; 	// Диаметр мячика
		   int    getXPos();
		   void   setXPos(int x);
		   void   setXRealPos(int x);
		   void   calculateXRealPos(int Frame_Width, int Frame_Height);
		   float  getXRealPos();
		   int    getYPos();
		   void   setYPos(int y);
		   void   setYRealPos(int y);
		   void   calculateYRealPos(int Frame_Width, int Frame_Height);
		   float  getYRealPos();
		   int    getZDiameter();
		   void   setZDiameter(int z);
		   void   setZRealPos(int z);
		   void   calculateZRealPos();
		   float  getZRealPos();
	   
		   VecD   staticXData;
		   VecD   staticZData;
		   VecD   staticYData;


private:
 
	float         	xPos, yPos, zDiameter, XRealPos, YRealPos, ZRealPos;

};

