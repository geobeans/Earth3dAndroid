#ifndef __SPECIALGEOMUTIL_H__
#define __SPECIALGEOMUTIL_H__

#include <vector>
using namespace std;

class ArrowPoint;
class BezierPoint;
class ArrowHead;
class ArrowStyle;
class SpecialGeomUtil
{
public:
	SpecialGeomUtil(void);
	~SpecialGeomUtil(void);

	static std::vector<ArrowPoint*> getEncirclementPtsFromPts(std::vector<ArrowPoint*> pts);
	static std::vector<ArrowPoint*> getEncirclementPtsFromThreePoint(ArrowPoint* pt1,ArrowPoint* pt2, ArrowPoint* pt3);
	static std::vector<ArrowPoint*> getFourPts(ArrowPoint* pt1,ArrowPoint* pt2,ArrowPoint* pt3);

	static std::vector<ArrowPoint*> getPtsFromThreePoint(ArrowPoint* pt,ArrowPoint* ptStart,ArrowPoint* ptEnd);
    static std::vector<BezierPoint*> getEncirlcementBezierPtsFromPts(ArrowPoint* ptElevation, ArrowPoint* ptStart,ArrowPoint* ptEnd);
	

	static ArrowHead* getLimitedArrowHead(ArrowPoint* ptStart,ArrowPoint* ptEnd,double ratio ,double maxLength,ArrowStyle arrowStyle);

	static void getArrowHeadPts(std::vector<ArrowPoint*> &pts,ArrowPoint* ptStart,ArrowPoint* ptEnd,ArrowStyle* arrowStyle,double ratio);

	static void getLimitedArrowHeadPts(std::vector<ArrowPoint*> &pts, ArrowPoint* ptStart,ArrowPoint* ptEnd,double ratio,double maxLength,ArrowStyle* arrowStyle);
	static	ArrowHead* getArrowHead(ArrowPoint* ptStart,ArrowPoint* ptEnd, ArrowStyle* arrowStyle);
	static ArrowPoint* getLineMidCtrPoint(ArrowPoint* ptStart,ArrowPoint* ptEnd,bool bLeft);

	static std::vector<ArrowPoint*> getDoubleArrowPts(ArrowPoint* ptStart1,ArrowPoint* ptStart2, ArrowPoint* ptEnd1,ArrowPoint* ptEnd2,ArrowStyle* arrowStyle);

	static void getMidCurve(std::vector<ArrowPoint*> &pts,ArrowPoint* ptLeft,ArrowPoint* ptLeftLine,ArrowPoint* ptMid,ArrowPoint* ptMidLine,
			ArrowPoint* ptRight,ArrowPoint* ptRightLine,bool isSameSide1,bool isSameSide2);

	static	ArrowPoint* getMidAnchorPoint(ArrowPoint* ptStart1, ArrowPoint* ptStart2,ArrowPoint* ptEnd1, ArrowPoint* ptEnd2,bool isSameSide);

	static std::vector<ArrowPoint*> getLineArrowPts(ArrowPoint* ptStart,ArrowPoint* ptEnd,ArrowStyle* arrowStyle,double tail, double head, double ratio);
	static	std::vector<ArrowPoint*> SpecialGeomUtil::getThreePointArrowPts(ArrowPoint* ptStart1,ArrowPoint* ptStart2,ArrowPoint* ptEnd,ArrowStyle* arrowStyle);

	static std::vector<ArrowPoint*> getCurveArrowPts(std::vector<ArrowPoint*> pts,ArrowStyle* arrowStyle,double ratio);
	static	std::vector<ArrowPoint*> getHasTailCurveArrowPts(std::vector<ArrowPoint*> pts,ArrowStyle* arrowStyle);

	static std::vector<ArrowPoint*> getThreeArrowPts(ArrowPoint* ptStart1,ArrowPoint* ptStart2, ArrowPoint* ptEnd1,ArrowPoint* ptEnd2,ArrowPoint* ptEndMid,ArrowStyle* arrowStyle);



};

#endif