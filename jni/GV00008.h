#ifndef __GV00008_H__
#define __GV00008_H__

#include "GVGeometry.h"
#include "Bezier.h"

class GV00008 : public GVGeometry
{
public:
	GV00008(int arrownum);
	~GV00008();

	int m_arrowNum;
	int getControlPointCount();
	void getControlPoints(std::vector<GVCoord>& vtxBuffer);
	GVCoord getControlPoint(int idx);
	void setControlPointsEx(std::vector<GVCoord>& vtxBuffer);
	void setControlPoints(std::vector<GVCoord>& vtxBuffer);
	void setControlPoint(int idx, const GVCoord& vtx);
	bool toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2,osgEarth::MapNode* mapNode);
	int getGeometryType();
private:
	std::vector<GVCoord> _controlPoints;
};

#endif //__GV00008_H__