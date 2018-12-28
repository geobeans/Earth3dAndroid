
#include "GeomUtil.h"
#include "ArrowPoint.h"
#include "BezierPoint.h"
#include <math.h>
GeomUtil::GeomUtil(void)
{
}


GeomUtil::~GeomUtil(void)
{
}


/**求直线的交点
	 */
ArrowPoint* GeomUtil::getLineIntersection(ArrowPoint* a1, ArrowPoint* a2, ArrowPoint*  b1,ArrowPoint* b2)
{
	double k1 = (a2->y-a1->y) / (a2->x-a1->x);
	double k2 = (b2->y-b1->y) / (b2->x-b1->x);

	if( k1 == k2 ) return NULL;
	double x,y;
	double m1,m2;

		//if( !isFinite(k1) )
	//if(k1==Double.NaN)
	
	if(!isfinite(k1))
	//if(!_finite(k1))
	{
		x = a1->x;
		m2 = b1->y - k2 * b1->x;
		y = k2 * x + m2;

	} 
	else 
		if ( !isfinite(k2) )
	//if(k2==Double.NaN)
		//if(!_finite(k2))
		{
			m1 = a1->y - k1 * a1->x;
			x = b1->x;
			y = k1 * x + m1;

		}
		else 
		{
			m1 = a1->y - k1 * a1->x;
			m2 = b1->y - k2 * b1->x;				
			x = (m1-m2) / (k2-k1);
			y = k1 * x + m1;
			
		}
		return new ArrowPoint(x,y);
}

bool GeomUtil::isRight(ArrowPoint pt,ArrowPoint ptStart,ArrowPoint ptEnd)
{
		ArrowPoint* vec1 = ptEnd.subtract(&ptStart);
		ArrowPoint* vec2 = pt.subtract(&ptStart);
		bool flag=true;
		if(vec1->x*vec2->y - vec1->y*vec2->x > 0)
			flag= true;
		else
			flag= false;
		delete vec1;
		delete vec2;
		return flag;
}	 

void GeomUtil::getSingleBezierCurvePts(std::vector<ArrowPoint*> &pts,ArrowPoint* ptStart, ArrowPoint* ptEnd,ArrowPoint* ptStartCtr,ArrowPoint* ptEndCtr,double density)
{
		//根据距离优化分割次数
		double dis1 = ArrowPoint::distance(ptStart,ptEnd);
		double dis2 = ArrowPoint::distance(ptStart,ptStartCtr);
		double dis3 = ArrowPoint::distance(ptEnd,ptEndCtr);
		
		double maxDis = max(max(dis1,dis2),dis3);
		//double num = sqrt(maxDis)*density;
		double num =40;
		
		double x = ptStart->x;
		double y = ptStart->y;
		for(double i=0;i<=1;i+=1/num)
		{
			x = pow(i,3)*(ptEnd->x+3*(ptStartCtr->x-ptEndCtr->x)-ptStart->x)
					+3*pow(i,2)*(ptStart->x-2*ptStartCtr->x+ptEndCtr->x)
					+3*i*(ptStartCtr->x-ptStart->x)+ptStart->x;
			y = pow(i,3)*(ptEnd->y+3*(ptStartCtr->y-ptEndCtr->y)-ptStart->y)
					+3*pow(i,2)*(ptStart->y-2*ptStartCtr->y+ptEndCtr->y)
					+3*i*(ptStartCtr->y-ptStart->y)+ptStart->y;
			pts.push_back(new ArrowPoint(x,y));
		}	
}


void GeomUtil::getQuadraticBezierPts(std::vector<ArrowPoint*> &pts,ArrowPoint* ptStart,ArrowPoint* ptEnd,ArrowPoint* ptCtrl,double density)
{
		double x,y;
		double dis1 = ArrowPoint::distance(ptStart,ptEnd);
		double dis2 = ArrowPoint::distance(ptStart,ptCtrl);
		double dis3 = ArrowPoint::distance(ptEnd,ptCtrl);
		double maxDis = max(max(dis1,dis2),dis3);
		double num = sqrt(maxDis)*density;
		if(num<10) num=40;
		for(double t = 0; t <= 1; t += 1/num)
		{
			x = (1-t)*(1-t)* ptStart->x + 2* t*(1-t)* ptCtrl->x + t * t * ptEnd->x;
			y = (1-t)*(1-t)* ptStart->y + 2* t*(1-t)* ptCtrl->y + t * t * ptEnd->y;
			pts.push_back(new ArrowPoint(x,y));
		}
		pts.push_back(new ArrowPoint(ptEnd->x,ptEnd->y));
}	

ArrowPoint*  GeomUtil::getSymmetryPoint(ArrowPoint* pt,ArrowPoint* ptStart,ArrowPoint* ptEnd)
{
		ArrowPoint* vec = ptEnd->subtract(ptStart);
		ArrowPoint* vecNorm = new ArrowPoint(-vec->y,vec->x);
		ArrowPoint* ptTmp= pt->add(vecNorm);
		ArrowPoint* ptCross = GeomUtil::getLineIntersection(ptStart,ptEnd,pt,ptTmp);
		ArrowPoint* ptCur = ArrowPoint::interpolate(ptStart,ptEnd,0.5);
		ArrowPoint* vec1 = ptCur->subtract(ptCross);

		ArrowPoint* res0=pt->add(vec1);
		ArrowPoint* res=res0->add(vec1);
		delete res0;
		delete vec1;
		delete ptCur;
		delete ptCross;
		delete ptTmp;
		delete vecNorm;
		delete vec;
		return res;
}

BezierPoint* GeomUtil::getBezierPoint(ArrowPoint* prePoint,ArrowPoint* curPoint,ArrowPoint* nextPoint)
{
		double dis1 = ArrowPoint::distance(prePoint,curPoint);
		double dis2 = ArrowPoint::distance(curPoint,nextPoint);
		ArrowPoint* vec = nextPoint->subtract(prePoint);
		vec->normalize(1);			
		
		BezierPoint* bezierPt = new BezierPoint(curPoint->x,curPoint->y);
		bezierPt->rightCtrPoint->x = curPoint->x - vec->x*dis1*0.3;
		bezierPt->rightCtrPoint->y = curPoint->y - vec->y*dis1*0.3;			
		bezierPt->leftCtrPoint->x = curPoint->x + vec->x*dis2*0.3;
		bezierPt->leftCtrPoint->y = curPoint->y + vec->y*dis2*0.3;
		delete vec;
		return bezierPt;
}

double GeomUtil::getPolylineLength(std::vector<ArrowPoint*> &pts)
{
		double length = 0;
		
		int len = pts.size();

		for(int i = 0 ; i < len - 1;i++)
		{
			ArrowPoint* p1=(ArrowPoint*)pts[i];
			ArrowPoint* p2=(ArrowPoint*)pts[i+1];
			length += ArrowPoint::distance(p1,p2);
		}
		return length;
}		
	
ArrowPoint* GeomUtil::getPointByRatio(std::vector<ArrowPoint*> &pts,double ratio)
{
		int len = pts.size();
		if(ratio < 0) return (ArrowPoint*)pts[0];
		if(ratio > 1) return (ArrowPoint*)pts[len-1];
		
		double length = GeomUtil::getPolylineLength(pts)*ratio;
		double sum = 0;
		ArrowPoint* pt=NULL;
		for(int i = 0; i < len - 1;i++)
		{
			double dis = ArrowPoint::distance((ArrowPoint*)pts[i],(ArrowPoint*)pts[i+1]);
			sum += dis;
			if(sum >= length)
			{
				pt = ArrowPoint::interpolate((ArrowPoint*)pts[i],(ArrowPoint*)pts[i+1],(sum - length)/dis);
				break;
			}
		}
		return pt;
	}	


void GeomUtil::getBogusParallelPts(std::vector<ArrowPoint*> &pts,std::vector<ArrowPoint*> &leftPts,std::vector<ArrowPoint*> &rightPts, double beginLength, double endLength,
											   bool isClosed, std::string joints, double miterLimit)
	{
		int len = pts.size();
		if(len < 2) return;
		ArrowPoint* pts0=(ArrowPoint*)pts[0];
		ArrowPoint* ptsend=(ArrowPoint*)pts[len-1];
		if(isClosed && !pts0->equals(ptsend))
		{
			pts.push_back(pts0->clone());
			len = pts.size();
		}
		
		double totalLength = GeomUtil::getPolylineLength(pts);
		double sum = 0;			
		ArrowPoint ptStart,ptEnd,ptCur;
		std::vector<ArrowPoint*> leftRightPts;
		
		pts0=(ArrowPoint*)pts[0];
		ArrowPoint* pts1=(ArrowPoint*)pts[1];
		ArrowPoint* ptlen_2=(ArrowPoint*)pts[len-2];
		ArrowPoint* ptlen_1=(ArrowPoint*)pts[len-1];
		for(int i = 0 ; i < len;i++)
		{
			if(i == 0)//处理起点
			{
				if(isClosed)
				{
					//leftRightPts = GeomUtil.getDiagonalPoints(pts[len-2],pts[0],pts[1],beginLength);
					leftRightPts = GeomUtil::getDiagonalPoints(ptlen_2,pts0,pts1,beginLength,"JointStyle.MITER",8);
					//leftPts.push(leftRightPts.pop());
					//rightPts.push(leftRightPts.pop());
				}
				else
				{
					//leftRightPts = GeomUtil.getVerticalPoints(pts[0],pts[1],beginLength);
					leftRightPts = GeomUtil::getVerticalPoints(pts0,pts1,beginLength);
					//leftPts.push(leftRightPts.pop());
					//rightPts.push(leftRightPts.pop());
				}
				//leftPts.add(leftRightPts.remove(leftRightPts.size()-1));
				//rightPts.add(leftRightPts.remove(leftRightPts.size()-1));
				ArrowPoint* tpt1=leftRightPts[leftRightPts.size()-1];
				leftRightPts.pop_back();
				leftPts.push_back(tpt1);

				ArrowPoint* tpt2=leftRightPts[leftRightPts.size()-1];
				leftRightPts.pop_back();
				rightPts.push_back(tpt2);

				leftRightPts.clear();
			}
			else if(i == pts.size() - 1)//处理终点
			{
				if(isClosed)
				{
					//leftRightPts = GeomUtil.getDiagonalPoints(pts[len-2],pts[0], pts[1],beginLength);
					leftRightPts = GeomUtil::getDiagonalPoints(ptlen_2,pts0,pts1,beginLength,"JointStyle.MITER",8);
					//leftPts.push(leftRightPts.pop());
					//rightPts.push(leftRightPts.pop());
					//leftPts.add(leftRightPts.remove(leftRightPts.size()-1));
					//rightPts.add(leftRightPts.remove(leftRightPts.size()-1));
					ArrowPoint* tpt1=leftRightPts[leftRightPts.size()-1];
					leftRightPts.pop_back();
					leftPts.push_back(tpt1);

					ArrowPoint* tpt2=leftRightPts[leftRightPts.size()-1];
					leftRightPts.pop_back();
					rightPts.push_back(tpt2);
				}
				else
				{
					//leftRightPts = GeomUtil.getVerticalPoints(pts[pts.length-1],pts[pts.length-2],endLength);
					leftRightPts = GeomUtil::getVerticalPoints(ptlen_1,ptlen_2,endLength);
					//rightPts.push(leftRightPts.pop());
					//leftPts.push(leftRightPts.pop());
					//rightPts.add(leftRightPts.remove(leftRightPts.size()-1));
					//leftPts.add(leftRightPts.remove(leftRightPts.size()-1));
					ArrowPoint* tpt1=leftRightPts[leftRightPts.size()-1];
					leftRightPts.pop_back();
					rightPts.push_back(tpt1);

					ArrowPoint* tpt2=leftRightPts[leftRightPts.size()-1];
					leftRightPts.pop_back();
					leftPts.push_back(tpt2);
				}	
				leftRightPts.clear();
			}
			else//处理中间点
			{
				ArrowPoint* pts_i=(ArrowPoint*)pts[i];
				ArrowPoint* pts_i_1=(ArrowPoint*)pts[i-1];
				ArrowPoint* pts_iadd1=(ArrowPoint*)pts[i+1];
				//sum += Point.distance(pts[i],pts[i-1]);
				sum += ArrowPoint::distance(pts_i,pts_i_1);
				double length = beginLength - sum/totalLength*(beginLength - endLength);
				//leftRightPts = GeomUtil.getDiagonalPoints(pts[i - 1],pts[i],pts[i+1],length,joints,miterLimit);
				leftRightPts = GeomUtil::getDiagonalPoints(pts_i_1,pts_i,pts_iadd1,length,joints,miterLimit);
				//leftPts.push(leftRightPts.pop());
				//rightPts.push(leftRightPts.pop());
				//leftPts.add(leftRightPts.remove(leftRightPts.size()-1));
				//rightPts.add(leftRightPts.remove(leftRightPts.size()-1));
				ArrowPoint* tpt1=leftRightPts[leftRightPts.size()-1];
				leftRightPts.pop_back();
				leftPts.push_back(tpt1);

				ArrowPoint* tpt2=leftRightPts[leftRightPts.size()-1];
				leftRightPts.pop_back();
				rightPts.push_back(tpt2);
				leftRightPts.clear();
			}
		}
		
	}

std::vector<ArrowPoint*> GeomUtil::getDiagonalPoints(ArrowPoint* ptPre, ArrowPoint* ptCur,ArrowPoint* ptNext,double len,std::string joints ,double miterLimit)
	{
		//对角线方向上的矢量
		ArrowPoint* vec1 = ptPre->subtract(ptCur);
		ArrowPoint* vec2 = ptNext->subtract(ptCur);
		vec1->normalize(1);
		vec2->normalize(1);
		ArrowPoint* ptDiag = vec1->add(vec2);	
		ptDiag->normalize(1);
		
		//处理共线的情况
		if(fabs(ptDiag->x) < 0.001 && fabs(ptDiag->y) < 0.001)
		{
			delete vec1;
			delete vec2;
			delete ptDiag;
			return getVerticalPoints(ptCur,ptNext,len);
		}
		
		//对角线上一点
		/*switch(joints)
		{
			case "JointStyle.MITER":
				double cosQ = vec1.x*ptDiag.x + vec1.y*ptDiag.y;
				double lenDiag = len/Math.sqrt(1 - cosQ*cosQ); 
				//if(!isNaN(miterLimit) && lenDiag > miterLimit)
				if(! Double.isNaN(miterLimit) && lenDiag > miterLimit)
					ptDiag.normalize(miterLimit);
				else
					ptDiag.normalize(lenDiag);
				break;
			case "JointStyle.ROUND":
				ptDiag.normalize(len);
				break;
		}*/
		if(strcmp(joints.c_str(),"JointStyle.MITER")==0)
		{
			double cosQ = vec1->x*ptDiag->x + vec1->y*ptDiag->y;
			double lenDiag = len/sqrt(1 - cosQ*cosQ); 
			if(!isnan(miterLimit) && lenDiag > miterLimit)
			//if(! _isnan(miterLimit) && lenDiag > miterLimit)
				ptDiag->normalize(miterLimit);
			else
				ptDiag->normalize(lenDiag);
		}
		else if(strcmp(joints.c_str(),"JointStyle.ROUND")==0)
		{
			ptDiag->normalize(len);
		}
		//	
		ArrowPoint* pt = ptCur->add(ptDiag);
		
		std::vector<ArrowPoint*> pts;
		////根据点在ptPre->ptCur向量的左侧，右侧顺序添加到pts数组
		if(GeomUtil::isRight(*pt,*ptPre,*ptCur))
		{
			pts.push_back(pt);
			pts.push_back(ptCur->subtract(ptDiag));
		}
		else
		{
			pts.push_back(ptCur->subtract(ptDiag));
			pts.push_back(pt);	
		}
		delete vec1;
		delete vec2;
		delete ptDiag;
		return pts;
	}

	std::vector<ArrowPoint*> GeomUtil::getVerticalPoints(ArrowPoint* ptStart, ArrowPoint* ptEnd,double len)
	{
		//垂线上的矢量
		ArrowPoint* vec = ptEnd->subtract(ptStart);
		ArrowPoint* vert = new ArrowPoint(vec->y,-vec->x);
		vert->normalize(len);
		
		//ptStart->ptEnd方向左侧，与右侧的两点
		ArrowPoint* ptRight= ptStart->add(vert);
		ArrowPoint* ptLeft = ptStart->subtract(vert);
		
		//按ptStart->ptEnd方向的左侧右侧的顺序添加到pts中并返回
		std::vector<ArrowPoint*> pts ;
		pts.push_back(ptLeft);
		pts.push_back(ptRight);
		delete vec;
		delete vert;
		return pts;	
	}

	std::vector<ArrowPoint*> GeomUtil::getCurvePts(std::vector<ArrowPoint*> pts,bool isClosed )
	{
		std::vector<ArrowPoint*> curvePts ;
		if(pts.size() < 2) return curvePts;
		//2017-05-18 zhangjianbing修改
		if(pts.size() == 2) {
			curvePts.push_back(pts[0]);
			curvePts.push_back(pts[1]);
			return curvePts;
		}
		
		std::vector<BezierPoint*> bezierPts = getBezierPtsFromPts(pts,isClosed);					
		for(int i = 0;i < bezierPts.size() - 1;i++)
		{
			BezierPoint* ptStart = (BezierPoint*)bezierPts[i];
			BezierPoint* ptEnd = (BezierPoint*)bezierPts[i+1];
			getSingleBezierCurvePts(curvePts,ptStart->point,ptEnd->point,
				ptStart->leftCtrPoint,ptEnd->rightCtrPoint,1.0);
		}		

		bezierPts.clear();
		return curvePts;
	}	


	std::vector<BezierPoint*> GeomUtil::getBezierPtsFromPts(std::vector<ArrowPoint*> pts,bool isClosed)
	{
		std::vector<BezierPoint*> bezierPts ;
		
		int i;
		int len = pts.size();			
		if(isClosed)
		{
			ArrowPoint* ptsLen_1=(ArrowPoint*)pts[len-1];
			ArrowPoint* pts0=(ArrowPoint*)pts[0];
			ArrowPoint* pts1=(ArrowPoint*)pts[1];
			for(i = 0; i < len;i++)
			{
				ArrowPoint* pts_i=(ArrowPoint*)pts[i];

				if(i == 0)
				{
					//bezierPts.push(GeomUtil.getBezierPoint(pts[len-1],pts[i],pts[i+1]));
					ArrowPoint* pts_iadd1=(ArrowPoint*)pts[i+1];
					bezierPts.push_back(GeomUtil::getBezierPoint(ptsLen_1,pts_i,pts_iadd1));
				}
				else if(i == len - 1)
				{
					//bezierPts.push(GeomUtil.getBezierPoint(pts[i-1],pts[i],pts[0]));
					ArrowPoint* pts_i_1=(ArrowPoint*)pts[i-1];
					bezierPts.push_back(GeomUtil::getBezierPoint(pts_i_1,pts_i,pts0));
				}
				else
				{
					//bezierPts.push(GeomUtil.getBezierPoint(pts[i-1],pts[i],pts[i+1]));
					ArrowPoint* pts_i_1=(ArrowPoint*)pts[i-1];
					ArrowPoint* pts_iadd1=(ArrowPoint*)pts[i+1];
					bezierPts.push_back(GeomUtil::getBezierPoint(pts_i_1,pts_i,pts_iadd1));
				}
			}
			//bezierPts.push(GeomUtil.getBezierPoint(pts[len-1],pts[0],pts[1]));
			bezierPts.push_back(GeomUtil::getBezierPoint(ptsLen_1,pts0,pts1));
		}
		else
		{
			for(i = 0; i < len;i++)
			{
				ArrowPoint* pts_i=(ArrowPoint*)pts[i];

				if(i == 0 || i == len - 1)//处理起点和终点
					//bezierPts.push(new BezierPoint(pts[i].x,pts[i].y));
					bezierPts.push_back(new BezierPoint(pts_i->x,pts_i->y));
				else//处理中间点
					//bezierPts.push(GeomUtil.getBezierPoint(pts[i-1],pts[i],pts[i+1]));
				{
					ArrowPoint* pts_i_1=(ArrowPoint*)pts[i-1];
					ArrowPoint* pts_iadd1=(ArrowPoint*)pts[i+1];
					bezierPts.push_back(GeomUtil::getBezierPoint(pts_i_1,pts_i,pts_iadd1));
				}
			}
		}
		return bezierPts;
	}


double GeomUtil::distanceOfPointAndLine(ArrowPoint* pt,ArrowPoint* pt1,ArrowPoint* pt2)
{
		double dis;
		//通过内积判断夹角为锐角或钝角
		if((pt->x - pt1->x)*(pt2->x - pt1->x) + (pt->y - pt1->y)*(pt2->y - pt1->y) < 0)
			dis = ArrowPoint::distance(pt,pt1);
		else if((pt->x - pt2->x)*(pt1->x - pt2->x) + (pt->y - pt2->y)*(pt1->y - pt2->y) < 0)
			dis = ArrowPoint::distance(pt,pt2);
		else
		{
			ArrowPoint* vec1= pt->subtract(pt1);
			ArrowPoint* vec2 = pt->subtract(pt2);
			double area = fabs(vec1->x*vec2->y - vec1->y*vec2->x);//根据叉积计算面积
			double disLine = ArrowPoint::distance(pt1,pt2);
			if(disLine == 0)
				dis = ArrowPoint::distance(pt,pt1);	
			else
				dis = area/disLine;
			delete vec1;
			delete vec2;
		}
		
		return dis;
	}

std::vector<ArrowPoint*> GeomUtil::getPtsFromBezierPts(std::vector<BezierPoint*> bezierPts,double density,bool isClosed)
{
		std::vector<ArrowPoint*> pts ;
		
		int len = bezierPts.size();
		for(int i = 0; i < len - 1;i++)
		{
			BezierPoint* start = (BezierPoint*)bezierPts[i];
		    BezierPoint* end = (BezierPoint*)bezierPts[i+1];
			GeomUtil::getSingleBezierCurvePts(pts,start->point,end->point,start->rightCtrPoint
				,end->leftCtrPoint,density);
		}
		if(isClosed)
			GeomUtil::getSingleBezierCurvePts(pts,((BezierPoint*)bezierPts[len-1])->point,((BezierPoint*)bezierPts[0])->point,((BezierPoint*)bezierPts[len-1])->rightCtrPoint,((BezierPoint*)bezierPts[0])->leftCtrPoint,density);
		return pts;
	}
