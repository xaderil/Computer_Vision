#include "Ball.hpp"

Ball::Ball(string Color) {

	this->Color = Color;

	if (Color == "blue") {

		setHSVmin(Scalar(60, 50, 20));
		setHSVmax(Scalar(120, 256, 256));

	};
	if (Color == "red") {

		setHSVmin(Scalar(0, 70, 20));
		setHSVmax(Scalar(50, 256, 256));

	};
	if (Color == "green") {

		setHSVmin(Scalar(60, 70, 70));
		setHSVmax(Scalar(170, 256, 256));

	};
};

void Ball::setHSVmin(Scalar min) {
	this->HSVmin = min;
};
void Ball::setHSVmax(Scalar max) {
	this->HSVmax = max;
};

int Ball::getXPos() {
	return this->xPos;
}

void Ball::setXPos(int x) {
	this->xPos = x;

}
void Ball::setXRealPos(int x) {
	this->XRealPos = x;
};

int Ball::getYPos() {

	return this->yPos;

}

void Ball::setYPos(int y) {

	this->yPos = y;

};
void Ball::setYRealPos(int y) {
	this->YRealPos = y;
};

int Ball::getZDiameter() {
	return this->zDiameter;
};

void Ball::setZDiameter(int z) {
	this->zDiameter = z;
};

void Ball::setZRealPos(int z) {
	this->ZRealPos = z;
};


Scalar Ball::getHSVmin() {

	return this->HSVmin;

}
Scalar Ball::getHSVmax() {

	return this->HSVmax;
}

/// Нахождение реального X объекта
void Ball::calculateXRealPos(int Frame_Width, int Frame_Height) {
	int Cu = Frame_Width/2;
	int Cv = Frame_Height/2;
	this->XRealPos = ((xPos-Cu)/f)*ZRealPos;
	this->YRealPos = ((yPos-Cv)/f)*ZRealPos;
};
/// Нахождение реального Y объекта
void Ball::calculateYRealPos(int Frame_Width, int Frame_Height) {

};

/// Нахождение реального Z объекта
void Ball::calculateZRealPos() {
	float Z = (1/(this->zDiameter))*(this->f)*(this->d);
	this->ZRealPos = Z;
	// cout << Z << endl;
};
