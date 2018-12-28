#ifndef __GEOMUTIL_H__
#define __GEOMUTIL_H__

#include <vector>
#include <string>
using namespace std;

class ArrowPoint;
class BezierPoint;
class GeomUtil
{
public:
	GeomUtil(void);
	~GeomUtil(void);

	static ArrowPoint* getLineIntersection(ArrowPoint* a1, ArrowPoint* a2, ArrowPoint*  b1,ArrowPoint* b2);
	static bool isRight(ArrowPoint pt,ArrowPoint ptStart,ArrowPoint ptEnd);

	static void getSingleBezierCurvePts(std::vector<ArrowPoint*> &pts,ArrowPoint* ptStart, ArrowPoint* ptEnd,ArrowPoint* ptStartCtr,ArrowPoint* ptEndCtr,double density);
	static void getQuadraticBezierPts(std::vector<ArrowPoint*> &pts,ArrowPoint* ptStart,ArrowPoint* ptEnd,ArrowPoint* ptCtrl,double density);
	static ArrowPoint*  getSymmetryPoint(ArrowPoint* pt,ArrowPoint* ptStart,ArrowPoint* ptEnd);

	static BezierPoint* getBezierPoint(ArrowPoint* prePoint,ArrowPoint* curPoint,ArrowPoint* nextPoint);
	static double getPolylineLength(std::vector<ArrowPoint*> &pts);

	static ArrowPoint* getPointByRatio(std::vector<ArrowPoint*> &pts,double ratio);

	static void getBogusParallelPts(std::vector<ArrowPoint*> &pts,std::vector<ArrowPoint*> &leftPts,std::vector<ArrowPoint*> &rightPts, double beginLength, double endLength,
											   bool isClosed, std::string joints, double miterLimit);

	static std::vector<ArrowPoint*> getDiagonalPoints(ArrowPoint* ptPre, ArrowPoint* ptCur,ArrowPoint* ptNext,double len,std::string joints ,double miterLimit);
	static std::vector<ArrowPoint*> getVerticalPoints(ArrowPoint* ptStart, ArrowPoint* ptEnd,double len);

	static std::vector<ArrowPoint*> getCurvePts(std::vector<ArrowPoint*> pts,bool isClosed );
	static std::vector<BezierPoint*> getBezierPtsFromPts(std::vector<ArrowPoint*> pts,bool isClosed);
	static double distanceOfPointAndLine(ArrowPoint* pt,ArrowPoint* pt1,ArrowPoint* pt2);

	static std::vector<ArrowPoint*> getPtsFromBezierPts(std::vector<BezierPoint*> bezierPts,double density,bool isClosed);

};

#endif
