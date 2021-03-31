#include "Ball.hpp"

Ball::Ball(string Color) {

	this->Color = Color;

	if (Color == "blue") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(60, 50, 20));
		setHSVmax(Scalar(120, 256, 256));


	};
	if (Color == "red") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 70, 20));
		setHSVmax(Scalar(50, 256, 256));


	};
	if (Color == "green") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

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

int Ball::getYPos() {

	return this->yPos;

}

void Ball::setYPos(int y) {

	this->yPos = y;

}

Scalar Ball::getHSVmin() {

	return this->HSVmin;

}
Scalar Ball::getHSVmax() {

	return this->HSVmax;
}