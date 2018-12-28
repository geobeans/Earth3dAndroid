#include "GMatrix.h"
#include "ArrowPoint.h"
#include <math.h>
GMatrix::GMatrix(void)
{
	a=1;
    b=0;
    c=0;
    d=1;
    e=0;
    f=0;
}

GMatrix::GMatrix(double a, double b, double c, double d, double e,double f)
{
    	this->a=a;
    	this->b=b;
    	this->c=c;
    	this->d=d;
    	this->e=e;
    	this->f=f;
}
GMatrix::~GMatrix(void)
{

}

void GMatrix::identity() 
{
    	a=1;
    	c=0;
    	e=0;
    	b=0;
    	d=1;
    	f=0;
}

GMatrix* GMatrix::inverse()
{
    double denom = a * d - b * c;
    double a1=d / denom;
	double b1= b / -denom;
	double c1=c / -denom;
	double d1=a / denom;
	double e1=(d * e - c * f) / -denom;
	double f1= (b * e - a * f) / denom;
	return new GMatrix(a1,b1,c1,d1,e1,f1);
}
    
GMatrix* GMatrix::translate(double tx,double ty ) 
{
    double a1=1;
    double c1=0;
    double e1=tx;
    double b1=0;
    double d1=1;
    double f1=ty;
    return new GMatrix(a1,b1,c1,d1,e1,f1);
}
    
GMatrix* GMatrix::scale(double sx,double sy )
{
    double a1=sx;
    double c1=0;
    double e1=0;
    double b1=0;
   	double d1=sy;
   	double f1=0;
   	return new GMatrix(a1,b1,c1,d1,e1,f1);
}
    
    
GMatrix* GMatrix::rotate(double angle)
{
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);
    	
    double a1=cosAngle;
    double c1=-sinAngle;
    double e1=0;
    double b1=sinAngle;
    double d1=cosAngle;
    double f1=0;
    GMatrix* rotationMatrix = new GMatrix(a1,b1,c1,d1,e1,f1);
    return rotationMatrix;
}

GMatrix* GMatrix::multiply(GMatrix* m1, GMatrix* m2)
{
	double a=m1->a * m2->a + m1->c * m2->b;
	double c=m1->a * m2->c + m1->c * m2->d;
	double e= m1->a * m2->e + m1->c * m2->f + m1->e;
	double b= m1->b * m2->a + m1->d * m2->b;
	double d= m1->b * m2->c + m1->d * m2->d;
	double f=m1->b * m2->e + m1->d * m2->f + m1->f;
	return new GMatrix(a,b,c,d,e,f);
}

ArrowPoint* GMatrix::transformPoint(ArrowPoint point)
{
  	  double x=this->a * point.x + this->c * point.y + this->e;
  	  double y=this->b * point.x + this->d * point.y + this->f;
  	  return new ArrowPoint(x,y);
}