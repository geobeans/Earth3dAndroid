#ifndef __GV00010_H__
#define __GV00010_H__

#include "GVGeometry.h"
#include "Bezier.h"

class GV00010 : public GVGeometry
{
public:
	GV00010(int ihasTail);
	~GV00010();
	int m_tail;

	int getControlPointCount();
	void getControlPoints(std::vector<GVCoord>& vtxBuffer);
	GVCoord getControlPoint(int idx);
	void setControlPointsEx(std::vector<GVCoord>& vtxBuffer);
	void setControlPoints(std::vector<GVCoord>& vtxBuffer);
	void setControlPoint(int idx, const GVCoord& vtx);
	bool toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2,osgEarth::MapNode* mapNode);
	bool toVertice2(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2,osgEarth::MapNode* mapNode, osgViewer::View* viewer);
	int getGeometryType();
private:
	std::vector<GVCoord> _controlPoints;
};

#endif //__GV00010_H__