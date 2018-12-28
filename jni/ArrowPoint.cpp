#include "ArrowPoint.h"
#include <math.h>
ArrowPoint::ArrowPoint(void)
{
	x=0;
	y=0;
}

ArrowPoint::ArrowPoint(double _x, double _y)
{
	x=_x;
	y=_y;
}

ArrowPoint::~ArrowPoint(void)
{
}


ArrowPoint* ArrowPoint::subtract(ArrowPoint* p1)
{
	ArrowPoint* p=new ArrowPoint();
	p->x=this->x-p1->x;
	p->y=this->y-p1->y;
	return p;
}
	
ArrowPoint* ArrowPoint::add(ArrowPoint* p1)
{
	ArrowPoint* p=new ArrowPoint();
	p->x=this->x+p1->x;
	p->y=this->y+p1->y;
	return p;
}

double ArrowPoint::length()
{
	return sqrt(this->x*this->x+this->y*this->y);
}
	
void ArrowPoint::normalize(double len)
{
	double len2=length();
	this->x=this->x*len/len2;
	this->y=this->y*len/len2;
}
	
 ArrowPoint* ArrowPoint::interpolate(ArrowPoint* p1, ArrowPoint* p2, double r)
{
	double x=p1->x*r+p2->x*(1-r);
	double y=p1->y*r+p2->y*(1-r);
	return new ArrowPoint(x,y);
}

double ArrowPoint::distance(ArrowPoint* p1, ArrowPoint* p2)
{
	return sqrt((p2->x-p1->x)*(p2->x-p1->x)+(p2->y-p1->y)*(p2->y-p1->y));
}

bool ArrowPoint::equals(ArrowPoint* p)
{
	if(fabs(p->x-this->x)<0.000001 && fabs(p->y-this->y)<0.000001)
	{
		return true;
	}
	return false;
}

ArrowPoint* ArrowPoint::clone()
{
	return new ArrowPoint(this->x,this->y);
}

void ArrowPoint::set(ArrowPoint* p_)
{
	this->x=p_->x;
	this->y=p_->y;
}
