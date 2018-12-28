#ifndef __GMATRIX_H__
#define __GMATRIX_H__
class ArrowPoint;
class GMatrix
{
public:
	GMatrix(void);
	GMatrix(double a, double b, double c, double d, double e,double f);
	~GMatrix(void);

	double a,b,c,d,e,f;

	void identity() ;
	GMatrix* inverse();
	GMatrix* translate(double tx,double ty );
	GMatrix* scale(double sx,double sy );
	GMatrix* rotate(double angle);
	GMatrix* multiply(GMatrix* m1, GMatrix* m2);
	ArrowPoint* transformPoint(ArrowPoint point);
};

#endif