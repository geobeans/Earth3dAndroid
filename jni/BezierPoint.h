#ifndef __BEZIERPOINT_H__
#define __BEZIERPOINT_H__
class ArrowPoint;
class BezierPoint
{
public:
	BezierPoint(void);
	~BezierPoint(void);

    ArrowPoint* point ;
    ArrowPoint* leftCtrPoint;
    ArrowPoint* rightCtrPoint;

public:
    BezierPoint(double x, double y);
};
#endif

