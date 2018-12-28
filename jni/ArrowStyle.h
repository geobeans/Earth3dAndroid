#ifndef __ARROWSTYLE_H__
#define __ARROWSTYLE_H__
class ArrowStyle
{
public:
	ArrowStyle(void);
	~ArrowStyle(void);

	double headWidth;
	int headLength;
		
	double shaftThickness;
	double shaftPosition;

	double shaftControlPosition;

	double shaftControlSize;

	double edgeControlPosition;
	double edgeControlSize;
};
#endif
