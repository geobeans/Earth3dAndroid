#include "BezierPoint.h"
#include "ArrowPoint.h";

BezierPoint::BezierPoint(void)
{
	this->point = new ArrowPoint(0, 0);
    this->leftCtrPoint = new ArrowPoint(0, 0);
    this->rightCtrPoint = new ArrowPoint(0, 0);
}


BezierPoint::~BezierPoint(void)
{
	delete this->point;
	delete this->leftCtrPoint;
	delete this->rightCtrPoint;
}

BezierPoint::BezierPoint(double x, double y) 
{
    this->point = new ArrowPoint(x, y);
    this->leftCtrPoint = new ArrowPoint(x, y);
    this->rightCtrPoint = new ArrowPoint(x, y);
}
