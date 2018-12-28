#include "GV00010.h"
#include "ArrowPoint.h"
#include "SpecialGeomUtil.h"
#include "ArrowStyle.h"
#include "GeomUtil.h"


GV00010::GV00010(int ihasTail)
{
	m_tail=ihasTail;
}

GV00010::~GV00010()
{

}

int GV00010::getControlPointCount()
{
	std::vector<GVCoord> controlPoints;
	getControlPoints(controlPoints);
	return controlPoints.size();
}

void GV00010::getControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	if(_controlPoints.size() <= 0)
		return ;
	vtxBuffer = _controlPoints;
}

GVCoord GV00010::getControlPoint(int idx)
{
	if(idx >= 0 && idx < _controlPoints.size())
		return _controlPoints[idx];

	return GVCoord();
}

void GV00010::setControlPointsEx(std::vector<GVCoord> &vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00010::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00010::setControlPoint(int idx, const GVCoord& vtx)
{
	int cpSize = _controlPoints.size();
	if(idx < 0 || idx >= cpSize)
		return ;
	_controlPoints[idx] = vtx;

}

bool GV00010::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2,osgEarth::MapNode* mapNode)
{
	std::vector<GVCoord> bezierPoints;
	osg::Vec3d vecScreen;
	osg::Vec3d vecWorld;
	osg::Vec3d geoPt;
	osg::Vec3d geoPt22;
	std::vector<ArrowPoint*>  paas;

	const osgEarth::SpatialReference* geoSRS = mapNode->getMapSRS()->getGeographicSRS();
	
	ArrowStyle* style=new ArrowStyle();
		
   	for(int j =0; j < _controlPoints.size(); j++)
	{
		paas.push_back( new ArrowPoint(_controlPoints[j].lon,_controlPoints[j].lat));
	}
	std::vector<ArrowPoint*>  arr;
	if((m_tail==0||m_tail==1)&&_controlPoints.size()>=2)
	{
		if(m_tail==0)
			arr= SpecialGeomUtil::getLineArrowPts(paas[0], paas[1], new ArrowStyle(), 0.125, 0.3, 0.5);
		else if(m_tail==1)
		{
			style->headWidth = ArrowPoint::distance(paas[0], paas[1])*0.8;
			style->shaftPosition = 0;
		    arr= SpecialGeomUtil::getLineArrowPts(paas[0], paas[1],style,0.35,0.3,0.8);
		}
	}
	else if(m_tail==3&&_controlPoints.size()>=3)
	{
			arr = SpecialGeomUtil::getThreePointArrowPts(paas[0], paas[1],paas[2],style);
	}
	else if(m_tail==4&&_controlPoints.size()>=3)
	{
			arr = SpecialGeomUtil::getThreePointArrowPts(paas[0], paas[1],paas[2],style);
			ArrowPoint* mid = ArrowPoint::interpolate(paas[0], paas[1],0.5);
			ArrowPoint* ptTail = ArrowPoint::interpolate(mid,paas[2],0.8);
			arr.push_back(ptTail);
			delete mid;
	}
	else if(m_tail==5&&_controlPoints.size()>=3)
	{
			arr =SpecialGeomUtil::getCurveArrowPts(paas,style,0.07);
	}
		
	int PolyNum=arr.size();
	for(int i=0;i<PolyNum;i++)
	{
			ArrowPoint* pt=arr[i];
			bezierPoints.push_back(GVCoord(pt->x,pt->y,0));
	}

	/**/
	delete style;
	paas.clear();

	vtxBuffer = bezierPoints;
	vtxBuffer2->push_back(bezierPoints.size());

	return true;
}

bool GV00010::toVertice2(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2,osgEarth::MapNode* mapNode, osgViewer::View* viewer)
{
	std::vector<GVCoord> bezierPoints;
	osg::Vec3d vecScreen;
	osg::Vec3d vecWorld;
	osg::Vec3d geoPt;
	osg::Vec3d geoPt22;
	std::vector<ArrowPoint*>  paas;

	const osgEarth::SpatialReference* geoSRS = mapNode->getMapSRS()->getGeographicSRS();
	osg::ref_ptr<osg::Camera> camera = viewer->getCamera();
	osg::Matrix mVPW = camera->getViewMatrix() * camera->getProjectionMatrix() * camera->getViewport()->computeWindowMatrix();

	osg::Matrix invertVPW;
	invertVPW.invert(mVPW);
	ArrowStyle* style=new ArrowStyle();
		
   	for(int j =0; j < _controlPoints.size(); j++)
	{
		osg::Vec3d  v1=osg::Vec3d(_controlPoints[j].lon,_controlPoints[j].lat,0);//_controlPoints[count].alt
		osg::Vec3d worldIntPoint2;
		geoSRS->transformToWorld(v1,worldIntPoint2);
		vecScreen = worldIntPoint2 * mVPW;
		paas.push_back( new ArrowPoint(vecScreen.x(),vecScreen.y()));
	}
	std::vector<ArrowPoint*>  arr;
	if((m_tail==0||m_tail==1)&&_controlPoints.size()>=2)
	{
		if(m_tail==0)
			arr= SpecialGeomUtil::getLineArrowPts(paas[0], paas[1], new ArrowStyle(), 0.125, 0.3, 0.5);
		else if(m_tail==1)
		{
			style->headWidth = ArrowPoint::distance(paas[0], paas[1])*0.8;
			style->shaftPosition = 0;
		    arr= SpecialGeomUtil::getLineArrowPts(paas[0], paas[1],style,0.35,0.3,0.8);
		}
	}
	else if(m_tail==3&&_controlPoints.size()>=3)
	{
			arr = SpecialGeomUtil::getThreePointArrowPts(paas[0], paas[1],paas[2],style);
	}
	else if(m_tail==4&&_controlPoints.size()>=3)
	{
			arr = SpecialGeomUtil::getThreePointArrowPts(paas[0], paas[1],paas[2],style);
			ArrowPoint* mid = ArrowPoint::interpolate(paas[0], paas[1],0.5);
			ArrowPoint* ptTail = ArrowPoint::interpolate(mid,paas[2],0.8);
			arr.push_back(ptTail);
			delete mid;
			delete ptTail;
	}
	else if(m_tail==5&&_controlPoints.size()>=3)
	{
			arr =SpecialGeomUtil::getCurveArrowPts(paas,style,0.07);
	}
		
	int PolyNum=arr.size();
	for(int i=0;i<PolyNum;i++)
	{
			ArrowPoint* pt=arr[i];
			osg::Vec3d vecWorld;
			vecScreen=osg::Vec3d(pt->x,pt->y,0);

			osgUtil::LineSegmentIntersector::Intersections hits;
			if ( viewer->computeIntersections(vecScreen.x(), vecScreen.y(), hits) )
			{
				vecWorld = hits.begin()->getWorldIntersectPoint();
				geoSRS->transformFromWorld(vecWorld,geoPt22);
				bezierPoints.push_back(GVCoord(geoPt22.x(),geoPt22.y(),0));
			}
	}

		/**/
	delete style;
	paas.clear();

	vtxBuffer = bezierPoints;
	vtxBuffer2->push_back(bezierPoints.size());

	return true;
}

int GV00010::getGeometryType()
{
	return 10;
}
