
#include "GV00008.h"
#include "ArrowPoint.h"
#include "SpecialGeomUtil.h"
#include "ArrowStyle.h"
#include "GeomUtil.h"


GV00008::GV00008(int arrownum)
{
	m_arrowNum=arrownum;
}

GV00008::~GV00008()
{

}

int GV00008::getControlPointCount()
{
	std::vector<GVCoord> controlPoints;
	getControlPoints(controlPoints);
	return controlPoints.size();
}

void GV00008::getControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	if(_controlPoints.size() <= 0)
		return ;
	vtxBuffer = _controlPoints;
}

GVCoord GV00008::getControlPoint(int idx)
{
	if(idx >= 0 && idx < _controlPoints.size())
		return _controlPoints[idx];

	return GVCoord();
}

void GV00008::setControlPointsEx(std::vector<GVCoord> &vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00008::setControlPoints(std::vector<GVCoord>& vtxBuffer)
{
	_controlPoints = vtxBuffer;
}

void GV00008::setControlPoint(int idx, const GVCoord& vtx)
{
	int cpSize = _controlPoints.size();
	if(idx < 0 || idx >= cpSize)
		return ;
	_controlPoints[idx] = vtx;

}

#define M(row,col) m[col * 4 + row]
void Transform_Point(double out[4], const double m[16], const double in[4]){
    out[0] = M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
    out[1] = M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
    out[2] = M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
    out[3] = M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
}

osg::Vec3d WorldToScreen(osgViewer::View* view,osg::Vec3 worldpoint){
    double in[4], out[4];
    in[0] = worldpoint._v[0];
    in[1] = worldpoint._v[1];
    in[2] = worldpoint._v[2];
    in[3] = 1.0;
    //获得当前的投影矩阵和模型视图矩阵
    osg::Matrix projectMatrix= view->getCamera()->getProjectionMatrix();
    osg::Matrix viewprojectMatrix = view->getCamera()->getViewMatrix();
    //下面计算 模型视图矩阵 * 投影矩阵 * 视口窗口变换矩阵
    double modelViewMatrix[16];
    memcpy(modelViewMatrix,viewprojectMatrix.ptr(),sizeof(GLdouble) * 16);
    Transform_Point(out, modelViewMatrix, in);
    double myprojectMatrix[16];
    memcpy(myprojectMatrix,projectMatrix.ptr(),sizeof(GLdouble) * 16);
    Transform_Point(in, myprojectMatrix, out);
    if(int(in[3] * 100000) == 0){
       return osg::Vec3d(0,0,0);
    }
    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];
    int viewPort[4];
    osg::Viewport* myviewPort = view->getCamera()->getViewport();
    viewPort[0] = 0;
    viewPort[1] = 0;
    viewPort[2] = 800; //横向象素点
    viewPort[3] = 600;//纵向象素点
    //计算 三维点在屏幕上的二维投影点
    osg::Vec3d sceenPoint;
    sceenPoint._v[0] = (int)(viewPort[0] + (1 + in[0]) * viewPort[2] / 2 + 0.5);
    sceenPoint._v[1] = (int)(viewPort[1] + (1 + in[1]) * viewPort[3] / 2 + 0.5);
    sceenPoint._v[2] = 0;
    return sceenPoint;
}

bool GV00008::toVertice(std::vector<GVCoord>& vtxBuffer, std::vector<int>* vtxBuffer2,osgEarth::MapNode* mapNode)
{
	Bezier bezier;

	std::vector<GVCoord> bezierPoints;
	ArrowPoint paas[5]; 

	std::vector<ArrowPoint*>  arr22;
	if(_controlPoints.size()<4)
	{
		Bezier bezier;
		bezier.getBezier2(_controlPoints, &bezierPoints);
	}
	else if(_controlPoints.size()>=4 &&m_arrowNum==2)
	{
		ArrowStyle* style=new ArrowStyle();
		for (int k=0;k<4;k++)
		{
			paas[k] = ArrowPoint(0,0);
		}
		int count=0;
   		for(int j =0; j < 4; j++)
		{
			paas[count].x=_controlPoints[j].lon;
			paas[count].y=_controlPoints[j].lat;
			count++;
		}
		arr22=SpecialGeomUtil::getDoubleArrowPts(&paas[0],&paas[1],&paas[3],&paas[2],style);
		delete style;
	}
	else if(_controlPoints.size()==4 &&m_arrowNum==3)
	{
		ArrowStyle* style=new ArrowStyle();
		for (int k=0;k<4;k++)
		{
			paas[k] =ArrowPoint(_controlPoints[k].lon,_controlPoints[k].lat);
		}
		arr22=SpecialGeomUtil::getDoubleArrowPts(&paas[0],&paas[1],&paas[3],&paas[2],style);
		delete style;
	}
	else if(_controlPoints.size()>=5 &&m_arrowNum==3)
	{
		ArrowStyle* style=new ArrowStyle();
		for (int k=0;k<5;k++)
		{
			paas[k] =ArrowPoint(_controlPoints[k].lon,_controlPoints[k].lat);
		}
		arr22=SpecialGeomUtil::getThreeArrowPts(&paas[0],&paas[1],&paas[4],&paas[2],&paas[3],style);
		delete style;
	}
	int PolyNum=arr22.size();
	//先进行化简处理

	for(int i=0;i<PolyNum;i++)
	{
		ArrowPoint* pt=arr22[i];
		bezierPoints.push_back(GVCoord(pt->x,pt->y,0));
	}

	vtxBuffer = bezierPoints;
	vtxBuffer2->push_back(bezierPoints.size());

	return true;
}

int GV00008::getGeometryType()
{
	return 8;
}