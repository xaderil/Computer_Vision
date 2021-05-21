#include "Ball.hpp"

int Ball::f = 940; 

int Ball::H_MIN = 0;
int Ball::H_MAX = 256;
int Ball::S_MIN = 0;
int Ball::S_MAX = 256;
int Ball::V_MIN = 0;
int Ball::V_MAX = 256;


Ball::Ball(string Color) {

	try {
		JSON_Worker::readHSV();
	} catch (exception err) {
		JSON_Worker::writeHSVFile();
	}


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


/// Нахождение реального X объекта
void Ball::calculateXRealPos(int Frame_Width, int Frame_Height) {
	float Cu = Frame_Width/2;
	this->XRealPos = ((xPos-Cu)/f)*ZRealPos;
};
/// Нахождение реального Y объекта
void Ball::calculateYRealPos(int Frame_Width, int Frame_Height) {
	float Cv = Frame_Height/2;
	this->YRealPos = ((yPos-Cv)/f)*ZRealPos;
};

/// Нахождение реального Z объекта
void Ball::calculateZRealPos() {
	float Z = (1/(this->zDiameter))*(this->f)*(this->d);
	this->ZRealPos = Z;
};

float Ball::getXRealPos() {
	return this->XRealPos;
};

float Ball::getYRealPos() {
	return this->YRealPos;
};

float Ball::getZRealPos() {
	return this->ZRealPos;
};