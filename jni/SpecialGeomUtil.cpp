
#include "SpecialGeomUtil.h"

#include "GeomUtil.h"
#include "ArrowPoint.h"
#include "BezierPoint.h"
#include "GMatrix.h"
#include "ArrowHead.h"
#include "ArrowStyle.h"
#include <math.h>
#include <algorithm>

#define M_PI 3.14159265358979323846
SpecialGeomUtil::SpecialGeomUtil(void)
{
}


SpecialGeomUtil::~SpecialGeomUtil(void)
{
}

std::vector<ArrowPoint*> SpecialGeomUtil::getPtsFromThreePoint(ArrowPoint* pt,ArrowPoint* ptStart,ArrowPoint* ptEnd)
{
		ArrowPoint* vec= ptEnd->subtract(ptStart);
		ArrowPoint* vert = new ArrowPoint(vec->y, -vec->x);
		
		double dis = GeomUtil::distanceOfPointAndLine(pt,ptStart,ptEnd);			
		vert->normalize(dis);
		
		ArrowPoint* ptCur = ArrowPoint::interpolate(ptStart,ptEnd,0.5);
		
		std::vector<ArrowPoint*> pts ;
		if(GeomUtil::isRight(*pt,*ptStart,*ptEnd))
		{
			pts.push_back(ptEnd);
			pts.push_back(ptStart);	
			pts.push_back(ptCur->subtract(vert));
			pts.push_back(pt);
		}
		else
		{
			pts.push_back(ptStart);
			pts.push_back(ptEnd);
			pts.push_back(ptCur->add(vert));
			pts.push_back(pt);
		}
		delete ptCur;
		delete vert;
		delete vec;
		return pts;
	}	

std::vector<ArrowPoint*> SpecialGeomUtil::getEncirclementPtsFromPts(std::vector<ArrowPoint*> pts)
{
		int len = pts.size();
		ArrowPoint* pts0;
		ArrowPoint* pts1;
		ArrowPoint* pts2;
		ArrowPoint* ptFlag;
		ArrowPoint* ptMid;
		ArrowPoint* ptStart;
		ArrowPoint* ptEnd ;
		ArrowPoint* vec;
		ArrowPoint* vert;
		double dis;
		std::vector<ArrowPoint*>  pp;
		//����ͼ��
		switch(len)
		{	
			case 2:
				pts0=(ArrowPoint*)pts[0];
				pts1=(ArrowPoint*)pts[1];
				ptStart = pts0;
				ptEnd = pts1;
				vec = ptEnd->subtract(ptStart);
				vert = new ArrowPoint(vec->y, -vec->x);
				dis = ArrowPoint::distance(ptStart,ptEnd);
				ptMid = ArrowPoint::interpolate(ptStart,ptEnd,0.5);
				vert->normalize(dis/2);
				ptFlag = ptMid->add(vert);
				delete vec;
				delete vert;
				delete ptMid;
				pp=SpecialGeomUtil::getEncirclementPtsFromThreePoint(pts0,pts1,ptFlag);
				//delete ptFlag;
				return	pp;
			case 3:
				pts0=(ArrowPoint*)pts[0];
				pts1=(ArrowPoint*)pts[1];
				pts2=(ArrowPoint*)pts[2];
				pp=SpecialGeomUtil::getEncirclementPtsFromThreePoint(pts0,pts1,pts2);
				return	pp;
				//ArrayList ptsTmp = GraphicsUtil.getEncirclementPtsFromThreePoint(pts[0],pts[1],pts[2]);
			default:
				break;
		}	
		
		return pp;
	}	

std::vector<ArrowPoint*> SpecialGeomUtil::getEncirclementPtsFromThreePoint(ArrowPoint* pt1,ArrowPoint* pt2, ArrowPoint* pt3)
	{
		std::vector<ArrowPoint*> fourPts = SpecialGeomUtil::getFourPts(pt1,pt2,pt3);			
		std::vector<BezierPoint*> bezierPts = SpecialGeomUtil::getEncirlcementBezierPtsFromPts((ArrowPoint*)fourPts[2],(ArrowPoint*)fourPts[0],(ArrowPoint*)fourPts[1]);	
		fourPts.clear();

		std::vector<ArrowPoint*> res=GeomUtil::getPtsFromBezierPts(bezierPts,3.0,true);
		bezierPts.clear();
		return 	res;
	}


std::vector<ArrowPoint*> SpecialGeomUtil::getFourPts(ArrowPoint* pt1,ArrowPoint* pt2,ArrowPoint* pt3)
	{
		double dis12 = ArrowPoint::distance(pt1,pt2);
		double dis23 = ArrowPoint::distance(pt2,pt3);
		double dis13 = ArrowPoint::distance(pt1,pt3);
		
		double maxVal =  max(max(dis12,dis23),dis13);
		
		std::vector<ArrowPoint*> fourPts;
		if(maxVal == dis12)
			fourPts = SpecialGeomUtil::getPtsFromThreePoint(pt3,pt1,pt2);
		else if(maxVal == dis13)
			fourPts = SpecialGeomUtil::getPtsFromThreePoint(pt2,pt1,pt3);
		else
			fourPts = SpecialGeomUtil::getPtsFromThreePoint(pt1,pt2,pt3);
		return fourPts;
	}
	
	std::vector<BezierPoint*> SpecialGeomUtil::getEncirlcementBezierPtsFromPts(ArrowPoint* ptElevation, ArrowPoint* ptStart,ArrowPoint* ptEnd)
	{
		std::vector<BezierPoint*> bezierPts ;			
		double len = ArrowPoint::distance(ptStart,ptEnd);			
		ArrowPoint* ptMid = ArrowPoint::interpolate(ptStart,ptEnd,0.5);			
		ArrowPoint* vec = ptEnd->subtract(ptStart);
		ArrowPoint* vert = new ArrowPoint(vec->y,-vec->x);			
		double dis = GeomUtil::distanceOfPointAndLine(ptElevation,ptStart,ptEnd);
		
		//left  
		vert->normalize(len/4);
		GMatrix* matrix = new GMatrix();
		matrix=matrix->rotate(-M_PI/8);
		ArrowPoint* ctr = matrix->transformPoint(*vert);
		
		BezierPoint* bezierPt = new BezierPoint(ptStart->x,ptStart->y);
		ArrowPoint* p1=ptStart->subtract(ctr);
		bezierPt->leftCtrPoint = new ArrowPoint(p1->x,p1->y);
		ArrowPoint* p2=ptStart->add(ctr);
		bezierPt->rightCtrPoint = new ArrowPoint(p2->x,p2->y);
		delete p1;
		delete p2;
		bezierPts.push_back(bezierPt);
		
		//Buttom
		ArrowPoint* ptButtom= ptElevation;
		vec->normalize(len/4);
		bezierPt = new BezierPoint(ptButtom->x,ptButtom->y);
		p1=ptButtom->subtract(vec);
		p2=ptButtom->add(vec);
		bezierPt->leftCtrPoint = new ArrowPoint(p1->x,p1->y);
		bezierPt->rightCtrPoint =  new ArrowPoint(p2->x,p2->y);
		delete p1;
		delete p2;
		bezierPts.push_back(bezierPt);				
		
		//right
		vert->normalize(len/4);

		delete matrix;
		matrix = new GMatrix();
		matrix =matrix->rotate(M_PI/8);
		ctr = matrix->transformPoint(*vert);
		delete matrix;
		bezierPt = new BezierPoint(ptEnd->x,ptEnd->y);

		p1=ptEnd->add(ctr);
		p2=ptEnd->subtract(ctr);
		bezierPt->leftCtrPoint = new ArrowPoint(p1->x,p1->y);
		bezierPt->rightCtrPoint = new ArrowPoint(p2->x,p2->y);
		delete p1;
		delete p2;
		bezierPts.push_back(bezierPt);
		
		//top			
		vert->normalize(sqrt(dis));			
		ArrowPoint* ptTop= ptMid->subtract(vert);
		vec->normalize(len/4);
		bezierPt = new BezierPoint(ptTop->x,ptTop->y);
		p1=ptTop->add(vec);
		p2=ptTop->subtract(vec);
		bezierPt->leftCtrPoint = new ArrowPoint(p1->x,p1->y);
		bezierPt->rightCtrPoint = new ArrowPoint(p2->x,p2->y);
		delete p1;
		delete p2;
		bezierPts.push_back(bezierPt);
		
		delete vec;
		delete ptTop;
		delete vert;
		delete ptEnd;
		delete ptMid;
		delete ctr;
		return bezierPts;
	}


	ArrowHead* SpecialGeomUtil::getLimitedArrowHead(ArrowPoint* ptStart,ArrowPoint* ptEnd,double ratio ,double maxLength, ArrowStyle arrowStyle)
	{
		ArrowPoint* vec= ptEnd->subtract(ptStart);
		
		//�����ͷ����㣬�����Ƽ�ͷ����󳤶�ΪmaxLength
		double len = vec->length()*ratio;
		if(len > maxLength && maxLength > 0)
			vec->normalize(maxLength);
		else
			vec->normalize(len);
		ArrowPoint* pt2 = ptEnd->subtract(vec);			
		
		//��ȡ��ͷ
		ArrowHead* arrowHead= getArrowHead(pt2,ptEnd,&arrowStyle);
		delete vec;
		delete pt2;
		return arrowHead;
	}


	void SpecialGeomUtil::getArrowHeadPts(std::vector<ArrowPoint*> &pts,ArrowPoint* ptStart,ArrowPoint* ptEnd,ArrowStyle* arrowStyle,double ratio)
	{
		if(arrowStyle == NULL)
			arrowStyle = new ArrowStyle();
		if(ptStart->equals(ptEnd))
			return;
		
		ArrowPoint* fullVect= ptEnd->subtract(ptStart);
		double halfWidth = (arrowStyle->headWidth != -1) ? arrowStyle->headWidth/2 : fullVect->length()/2;
		arrowStyle->shaftThickness = halfWidth/2;
		
		ArrowPoint* startNorm = new ArrowPoint(fullVect->y,-fullVect->x);
		startNorm->normalize(arrowStyle->shaftThickness*ratio);
		
		ArrowPoint* start1 = ptStart->add(startNorm);
		ArrowPoint* start2 = ptStart->subtract(startNorm);
		ArrowPoint* end1 = ptEnd->add(startNorm);
		ArrowPoint* end2 = ptEnd->subtract(startNorm);	
		
		startNorm->normalize(halfWidth);
		ArrowPoint* edge1 = ptStart->add(startNorm);
		ArrowPoint* edge2 = ptStart->subtract(startNorm);
		
		ArrowPoint* shaftCenter = ArrowPoint::interpolate(ptEnd,ptStart,arrowStyle->shaftPosition);
		ArrowPoint* inter1 = GeomUtil::getLineIntersection(start1,end1,shaftCenter,edge1);
		ArrowPoint* inter2 = GeomUtil::getLineIntersection(start2,end2,shaftCenter,edge2);
		
		//ArrowPoint* edgeCenter = ArrowPoint.interpolate(ptEnd,ptStart,arrowStyle->edgeControlPosition);
		//ArrowPoint* edgeNorm = new ArrowPoint(startNorm->x,startNorm->y);//startNorm.clone();
		//edgeNorm->normalize(halfWidth*arrowStyle->edgeControlSize);
		//ArrowPoint* edgeCtrl1= edgeCenter->add(edgeNorm);
		//ArrowPoint* edgeCtrl2 = edgeCenter->subtract(edgeNorm);
		delete fullVect;
		delete shaftCenter;
		delete end1;
		delete end2;
		delete start1;
		delete start2;
		delete startNorm;
		pts.push_back(inter1);
		pts.push_back(edge1);
		pts.push_back(ptEnd);
		pts.push_back(edge2);
		pts.push_back(inter2);
	}


	void SpecialGeomUtil::getLimitedArrowHeadPts(std::vector<ArrowPoint*> &pts, ArrowPoint* ptStart,ArrowPoint* ptEnd,double ratio,double maxLength,ArrowStyle* arrowStyle)
	{
		ArrowPoint* vec = ptEnd->subtract(ptStart);
		
		//�����ͷ����㣬�����Ƽ�ͷ����󳤶�ΪmaxLength
		double len = vec->length()*ratio;
		if(len > maxLength && maxLength > 0)
			vec->normalize(maxLength);
		else
			vec->normalize(len);
		ArrowPoint* pt2 = ptEnd->subtract(vec);			
		
		getArrowHeadPts(pts,pt2,ptEnd,arrowStyle,0.5);
		delete pt2;
		delete vec;
	}

	ArrowHead* SpecialGeomUtil::getArrowHead(ArrowPoint* ptStart,ArrowPoint* ptEnd, ArrowStyle* arrowStyle)
	{
		if(arrowStyle == NULL)
			arrowStyle = new ArrowStyle();
		if(ptStart->equals(ptEnd))
			return new ArrowHead(ptStart,ptStart,ptStart,ptStart,ptStart,ptStart,ptStart,ptStart,ptStart);
		
		ArrowPoint* fullVect = ptEnd->subtract(ptStart);
		double halfWidth = (arrowStyle->headWidth != -1) ? arrowStyle->headWidth/2 : fullVect->length()/2;
		arrowStyle->shaftThickness = halfWidth/2;
		
		ArrowPoint* startNorm = new ArrowPoint(fullVect->y,-fullVect->x);
		startNorm->normalize(arrowStyle->shaftThickness/2);
		
		ArrowPoint* start1 = ptStart->add(startNorm);
		ArrowPoint* start2 = ptStart->subtract(startNorm);
		ArrowPoint* end1 = ptEnd->add(startNorm);
		ArrowPoint* end2 = ptEnd->subtract(startNorm);	
		
		startNorm->normalize(halfWidth);
		ArrowPoint* edge1 = ptStart->add(startNorm);
		ArrowPoint* edge2 = ptStart->subtract(startNorm);
		
		ArrowPoint* shaftCenter = ArrowPoint::interpolate(ptEnd,ptStart,arrowStyle->shaftPosition);
		ArrowPoint* inter1 = GeomUtil::getLineIntersection(start1,end1,shaftCenter,edge1);
		ArrowPoint* inter2 = GeomUtil::getLineIntersection(start2,end2,shaftCenter,edge2);
		
		ArrowPoint* edgeCenter = ArrowPoint::interpolate(ptEnd,ptStart,arrowStyle->edgeControlPosition);
		ArrowPoint* edgeNorm =new ArrowPoint(startNorm->x,startNorm->y); //startNorm.clone();
		edgeNorm->normalize(halfWidth*arrowStyle->edgeControlSize);
		ArrowPoint* edgeCtrl1 = edgeCenter->add(edgeNorm);
		ArrowPoint* edgeCtrl2 = edgeCenter->subtract(edgeNorm);
		
		//System.out.println(start1+":"+start2+":"+inter1+":"+inter2+":"+edge1+":"+edge2+":"+edgeCtrl1+":"+edgeCtrl2+":"+ptEnd);
		delete fullVect;
		delete startNorm;
		delete end1;
		delete end2;
		delete edgeCenter;
		delete edgeNorm;
		return new ArrowHead(start1,start2,inter1,inter2,edge1,edge2,edgeCtrl1,edgeCtrl2,ptEnd);
	}
	

	ArrowPoint* SpecialGeomUtil::getLineMidCtrPoint(ArrowPoint* ptStart,ArrowPoint* ptEnd,bool bLeft)
	{
		ArrowPoint* vec = ptEnd->subtract(ptStart);
		ArrowPoint* norm;
		if(bLeft)
			norm  = new ArrowPoint(vec->y, -vec->x);
		else
			norm = new ArrowPoint(-vec->y,vec->x);
		
		ArrowPoint* ptCenter = ArrowPoint::interpolate(ptStart,ptEnd,0.5);
		double len = vec->length();
		//norm->normalize(sqrt(len*2));
		norm->x=norm->x/8;
		norm->y=norm->y/8;
		ArrowPoint* ptCtr = ptCenter->add(norm);
		delete vec;
		delete norm;
		delete ptCenter;
		return ptCtr;
	}


	std::vector<ArrowPoint*> SpecialGeomUtil::getDoubleArrowPts(ArrowPoint* ptStart1,ArrowPoint* ptStart2, ArrowPoint* ptEnd1,ArrowPoint* ptEnd2,ArrowStyle* arrowStyle)
	{
		std::vector<ArrowPoint*> arrowPts;
		//��ȡ������ͷ
		ArrowHead* arrowHead1 = getLimitedArrowHead(ptStart1,ptEnd1,0.3,80,*arrowStyle);
		ArrowHead* arrowHead2 = getLimitedArrowHead(ptStart2,ptEnd2,0.3,80,*arrowStyle);
		std::vector<ArrowPoint*> arrowHeadPts1 ;
		std::vector<ArrowPoint*> arrowHeadPts2 ;
		getLimitedArrowHeadPts(arrowHeadPts1,ptStart1,ptEnd1,0.3,80,arrowStyle);
		getLimitedArrowHeadPts(arrowHeadPts2,ptStart2,ptEnd2,0.3,80,arrowStyle);
		//�жϵ��ptStart1->ptStart2�ķ���
		bool bLeft1 = GeomUtil::isRight(*ptEnd1,*ptStart1,*ptStart2);
		bool bLeft2 = GeomUtil::isRight(*ptEnd2,*ptStart1,*ptStart2);		
		bool bLeft3 = GeomUtil::isRight(*arrowHead1->interLeft,*ptStart1,*ptStart2);
		bool bLeft4 = GeomUtil::isRight(*arrowHead1->interRight,*ptStart1,*ptStart2);
		bool bLeft5 = GeomUtil::isRight(*arrowHead2->interLeft,*ptStart1,*ptStart2);
		bool bLeft6 = GeomUtil::isRight(*arrowHead2->interRight,*ptStart1,*ptStart2);
		
		//ptStart1һ��ļ�ͷ������ptStart1������
		ArrowPoint* pt1;
		ArrowPoint* pt2;
		ArrowPoint* pt3;
		ArrowPoint* pt4;
		ArrowPoint* ptMid;
		ArrowPoint* ptMidLine;
		ArrowPoint* ptCtrl;
		if(bLeft1 == false)
		{
			pt1 = arrowHead1->interRight;
			pt2 = arrowHead1->startRight;
			
			ptCtrl = getLineMidCtrPoint(ptStart1,arrowHead1->interLeft,true);
			GeomUtil::getQuadraticBezierPts(arrowPts,ptStart1,arrowHead1->interLeft,ptCtrl,3);
			//arrowPts = arrowPts.concat(arrowHeadPts1);
			//arrowPts.addAll(arrowHeadPts1);
			arrowPts.insert(arrowPts.end(), arrowHeadPts1.begin(), arrowHeadPts1.end()); 
			delete ptCtrl;
		}
		else
		{	
			pt1 = arrowHead1->interLeft;
			pt2 = arrowHead1->startLeft;
			ptCtrl = getLineMidCtrPoint(ptStart1,arrowHead1->interRight,false);
			std::vector<ArrowPoint*> pts22;
			GeomUtil::getQuadraticBezierPts(pts22,ptStart1,arrowHead1->interRight,ptCtrl,3);
			//arrowPts = arrowPts.concat(pts22);
			//arrowPts.addAll(pts22);
			arrowPts.insert(arrowPts.end(), pts22.begin(), pts22.end());
			//arrowPts = arrowPts.concat(arrowHeadPts1.reverse());
			//Collections.reverse(arrowHeadPts1);
			//arrowHeadPts1.reserve();
			std::reverse(arrowHeadPts1.begin(),arrowHeadPts1.end());
			//arrowPts.addAll(arrowHeadPts1);
			arrowPts.insert(arrowPts.end(), arrowHeadPts1.begin(), arrowHeadPts1.end());
			delete ptCtrl;
		}
		ArrowPoint* vec = ptStart1->subtract(ptStart2);
		
		if(bLeft2 == true)				
		{	
			//�м�����
			pt3 = arrowHead2->interRight;
			pt4 = arrowHead2->startRight;
			ptMid = getMidAnchorPoint(ptStart1,ptStart2,pt1,pt3,bLeft1==bLeft2);
			ptMidLine = ptMid->add(vec);				
			getMidCurve(arrowPts,pt1,pt2,ptMid,ptMidLine,pt3,pt4,bLeft3 == bLeft4,bLeft5 == bLeft6);
			//ptStart2һ��ļ�ͷ������ptStart2������
			
			//arrowPts = arrowPts.concat(arrowHeadPts2.reverse());
			//Collections.reverse(arrowHeadPts2);
			//arrowHeadPts2.reserve();
			std::reverse(arrowHeadPts2.begin(),arrowHeadPts2.end());
			//arrowPts.addAll(arrowHeadPts2);
			arrowPts.insert(arrowPts.end(),arrowHeadPts2.begin(),arrowHeadPts2.end());


			ptCtrl = getLineMidCtrPoint(arrowHead2->interLeft,ptStart2,false);
			GeomUtil::getQuadraticBezierPts(arrowPts,arrowHead2->interLeft,ptStart2,ptCtrl,3);
			delete ptMid;
			delete ptMidLine;
			delete ptCtrl;
		}
		else
		{
			//�м�����
			pt3 = arrowHead2->interLeft;
			pt4 = arrowHead2->startLeft;
			ptMid = getMidAnchorPoint(ptStart1,ptStart2,pt1,pt3,bLeft1 == bLeft2);
			ptMidLine = ptMid->add(vec);				
			getMidCurve(arrowPts,pt1,pt2,ptMid,ptMidLine,pt3,pt4,bLeft3 == bLeft4,bLeft5 == bLeft6);
			
			//ptStart2һ��ļ�ͷ������ptStart2������
			//arrowPts = arrowPts.concat(arrowHeadPts2);
			//arrowPts.addAll(arrowHeadPts2);
			arrowPts.insert(arrowPts.end(),arrowHeadPts2.begin(),arrowHeadPts2.end());

			ptCtrl = getLineMidCtrPoint(arrowHead2->interRight,ptStart2,true);
			GeomUtil::getQuadraticBezierPts(arrowPts,arrowHead2->interRight,ptStart2,ptCtrl,3);
	
			delete ptMid;
			delete ptMidLine;
			delete ptCtrl;
		}		
		delete vec;
		return arrowPts;
	}
	
	std::vector<ArrowPoint*> SpecialGeomUtil::getThreeArrowPts(ArrowPoint* ptStart1,ArrowPoint* ptStart2, ArrowPoint* ptEnd1,ArrowPoint* ptEnd2,ArrowPoint* ptEndMid, ArrowStyle* arrowStyle)
	{
		std::vector<ArrowPoint*> arrowPts;
		//��ȡ������ͷ
		ArrowHead* arrowHead1 = getLimitedArrowHead(ptStart1,ptEnd1,0.3,80,*arrowStyle);
		ArrowHead* arrowHead2 = getLimitedArrowHead(ptStart2,ptEnd2,0.3,80,*arrowStyle);
		std::vector<ArrowPoint*> arrowHeadPts1 ;
		std::vector<ArrowPoint*> arrowHeadPts2 ;
		getLimitedArrowHeadPts(arrowHeadPts1,ptStart1,ptEnd1,0.3,80,arrowStyle);
		getLimitedArrowHeadPts(arrowHeadPts2,ptStart2,ptEnd2,0.3,80,arrowStyle);

		std::vector<ArrowPoint*> arrowHeadPtsMid ;
		ArrowPoint* ptStartMid=new ArrowPoint((ptStart1->x+ptStart2->x)/2,(ptStart1->y+ptStart2->y)/2);
		ArrowHead* arrowHeadMid = getLimitedArrowHead(ptStartMid,ptEndMid,0.3,80,*arrowStyle);
		getLimitedArrowHeadPts(arrowHeadPtsMid,ptStartMid,ptEndMid,0.3,80,arrowStyle);

		//�жϵ��ptStart1->ptStart2�ķ���
		bool bLeft1 = GeomUtil::isRight(*ptEnd1,*ptStart1,*ptStart2);
		bool bLeft2 = GeomUtil::isRight(*ptEnd2,*ptStart1,*ptStart2);		
		bool bLeft3 = GeomUtil::isRight(*arrowHead1->interLeft,*ptStart1,*ptStart2);
		bool bLeft4 = GeomUtil::isRight(*arrowHead1->interRight,*ptStart1,*ptStart2);
		bool bLeft5 = GeomUtil::isRight(*arrowHead2->interLeft,*ptStart1,*ptStart2);
		bool bLeft6 = GeomUtil::isRight(*arrowHead2->interRight,*ptStart1,*ptStart2);
		
		//ptStart1һ��ļ�ͷ������ptStart1������
		ArrowPoint* pt1;
		ArrowPoint* pt2;
		ArrowPoint* pt3;
		ArrowPoint* pt4;
		ArrowPoint* ptMid;
		ArrowPoint* ptMidLine;
		ArrowPoint* ptCtrl;

		//2018-06-18����
		ArrowPoint* pt1_Mid;
		ArrowPoint* pt2_Mid;
		if(bLeft1 == false)
		{
			pt1 = arrowHead1->interRight;
			pt2 = arrowHead1->startRight;
			
			pt1_Mid=arrowHeadMid->interRight;
			pt2_Mid=arrowHeadMid->startRight;

			ptCtrl = getLineMidCtrPoint(ptStart1,arrowHead1->interLeft,true);
			GeomUtil::getQuadraticBezierPts(arrowPts,ptStart1,arrowHead1->interLeft,ptCtrl,3);
			//arrowPts = arrowPts.concat(arrowHeadPts1);
			//arrowPts.addAll(arrowHeadPts1);
			arrowPts.insert(arrowPts.end(), arrowHeadPts1.begin(), arrowHeadPts1.end()); 
			delete ptCtrl;
		}
		else
		{	
			pt1 = arrowHead1->interLeft;
			pt2 = arrowHead1->startLeft;

			pt1_Mid=arrowHeadMid->interLeft;
			pt2_Mid=arrowHeadMid->startLeft;

			ptCtrl = getLineMidCtrPoint(ptStart1,arrowHead1->interRight,false);
			std::vector<ArrowPoint*> pts22;
			GeomUtil::getQuadraticBezierPts(pts22,ptStart1,arrowHead1->interRight,ptCtrl,3);
			//arrowPts = arrowPts.concat(pts22);
			//arrowPts.addAll(pts22);
			arrowPts.insert(arrowPts.end(), pts22.begin(), pts22.end());
			//arrowPts = arrowPts.concat(arrowHeadPts1.reverse());
			//Collections.reverse(arrowHeadPts1);
			//arrowHeadPts1.reserve();
			std::reverse(arrowHeadPts1.begin(),arrowHeadPts1.end());
			//arrowPts.addAll(arrowHeadPts1);
			arrowPts.insert(arrowPts.end(), arrowHeadPts1.begin(), arrowHeadPts1.end());
			delete ptCtrl;
		}
		ArrowPoint* vec = ptStart1->subtract(ptStart2);
		
		if(bLeft2 == true)				
		{	
			//����1
			pt3 = arrowHeadMid->interRight;
			pt4 = arrowHeadMid->startRight;
			ptMid = getMidAnchorPoint(ptStart1,ptStartMid,pt1,pt3,bLeft1==bLeft2);
			vec=ptStart1->subtract(ptStartMid);
			ptMidLine = ptMid->add(vec);	
			getMidCurve(arrowPts,pt1,pt2,ptMid,ptMidLine,pt3,pt4,bLeft3 == bLeft4,bLeft5 == bLeft6);
			
			std::reverse(arrowHeadPtsMid.begin(),arrowHeadPtsMid.end());
			arrowPts.insert(arrowPts.end(),arrowHeadPtsMid.begin(),arrowHeadPtsMid.end());

			//�м�����
			pt3 = arrowHead2->interRight;
			pt4 = arrowHead2->startRight;
			ptMid = getMidAnchorPoint(ptStartMid,ptStart2,pt1_Mid,pt3,bLeft1==bLeft2);
			ptMidLine = ptMid->add(vec);				
			getMidCurve(arrowPts,pt1_Mid,pt2_Mid,ptMid,ptMidLine,pt3,pt4,bLeft3 == bLeft4,bLeft5 == bLeft6);

			//ptStart2һ��ļ�ͷ������ptStart2������
			
			//arrowPts = arrowPts.concat(arrowHeadPts2.reverse());
			//Collections.reverse(arrowHeadPts2);
			//arrowHeadPts2.reserve();
			std::reverse(arrowHeadPts2.begin(),arrowHeadPts2.end());
			//arrowPts.addAll(arrowHeadPts2);
			arrowPts.insert(arrowPts.end(),arrowHeadPts2.begin(),arrowHeadPts2.end());

			ptCtrl = getLineMidCtrPoint(arrowHead2->interLeft,ptStart2,false);
			GeomUtil::getQuadraticBezierPts(arrowPts,arrowHead2->interLeft,ptStart2,ptCtrl,3);
			delete ptMid;
			delete ptMidLine;
			delete ptCtrl;
		}
		else
		{
			//����1
			pt3 = arrowHeadMid->interLeft;
			pt4 = arrowHeadMid->startLeft;
			ptMid = getMidAnchorPoint(ptStart1,ptStartMid,pt1,pt3,bLeft1 == bLeft2);
			vec=ptStart1->subtract(ptStartMid);
			ptMidLine = ptMid->add(vec);				
			getMidCurve(arrowPts,pt1,pt2,ptMid,ptMidLine,pt3,pt4,bLeft3 == bLeft4,bLeft5 == bLeft6);
			//arrowPts.addAll(arrowHeadPtsMid);
			arrowPts.insert(arrowPts.end(),arrowHeadPtsMid.begin(),arrowHeadPtsMid.end());

			//�м�����
			pt3 = arrowHead2->interLeft;
			pt4 = arrowHead2->startLeft;
			ptMid = getMidAnchorPoint(ptStartMid,ptStart2,pt1_Mid,pt3,bLeft1 == bLeft2);
			vec = ptStartMid->subtract(ptStart2);
			ptMidLine = ptMid->add(vec);				
			getMidCurve(arrowPts,pt1_Mid,pt2_Mid,ptMid,ptMidLine,pt3,pt4,bLeft3 == bLeft4,bLeft5 == bLeft6);

			
			//ptStart2һ��ļ�ͷ������ptStart2������
			//arrowPts = arrowPts.concat(arrowHeadPts2);
			//arrowPts.addAll(arrowHeadPts2);
			arrowPts.insert(arrowPts.end(),arrowHeadPts2.begin(),arrowHeadPts2.end());

			ptCtrl = getLineMidCtrPoint(arrowHead2->interRight,ptStart2,true);
			GeomUtil::getQuadraticBezierPts(arrowPts,arrowHead2->interRight,ptStart2,ptCtrl,3);
	
			delete ptMid;
			delete ptMidLine;
			delete ptCtrl;
		}		
		delete vec;
		return arrowPts;
	}

	void SpecialGeomUtil::getMidCurve(std::vector<ArrowPoint*> &pts,ArrowPoint* ptLeft,ArrowPoint* ptLeftLine,ArrowPoint* ptMid,ArrowPoint* ptMidLine,
			ArrowPoint* ptRight,ArrowPoint* ptRightLine,bool isSameSide1,bool isSameSide2)
	{
		ArrowPoint* ptCtrLeft;
		ArrowPoint* ptCtrRight; 
		if(isSameSide1)//��ͷ������inter���Ƿ�ͬ��
			ptCtrLeft = GeomUtil::getLineIntersection(ptLeft,ptLeftLine,ptMid,ptMidLine);
		else//
			ptCtrLeft = ptLeftLine;
		if(isSameSide2)//��ͷ������inter���Ƿ�ͬ��
			ptCtrRight= GeomUtil::getLineIntersection(ptRight,ptRightLine,ptMid,ptMidLine);
		else
			ptCtrRight = ptRightLine;
		if(ptCtrLeft!=NULL)
			GeomUtil::getQuadraticBezierPts(pts,ptLeft,ptMid,ptCtrLeft,1);
		if(ptCtrRight!=NULL)
			GeomUtil::getQuadraticBezierPts(pts,ptMid,ptRight,ptCtrRight,1);

		delete ptCtrLeft;
		delete ptCtrRight;

	}
	

	ArrowPoint* SpecialGeomUtil::getMidAnchorPoint(ArrowPoint* ptStart1, ArrowPoint* ptStart2,ArrowPoint* ptEnd1, ArrowPoint* ptEnd2,bool isSameSide)
	{
		ArrowPoint* ptMid1= ArrowPoint::interpolate(ptStart1,ptStart2,0.5);		
	    ArrowPoint* ptMid2 = ArrowPoint::interpolate(ptEnd1,ptEnd2,0.5);
		
	    ArrowPoint* vec = ptStart1->subtract(ptStart2);
	    ArrowPoint* pt1 = ptEnd1->add(vec);
	    ArrowPoint* pt2 = ptEnd2->add(vec);
	    ArrowPoint* pt3 = GeomUtil::getLineIntersection(pt1,ptEnd1,ptMid1,ptMid2);
	    ArrowPoint* pt4 = GeomUtil::getLineIntersection(pt2,ptEnd2,ptMid1,ptMid2);
		
		double dis1 = ArrowPoint::distance(ptMid1,pt3);
		double dis2 = ArrowPoint::distance(ptMid1,pt4);
		
		bool bLeft1 = GeomUtil::isRight(*ptEnd1,*ptStart1,*ptStart2);
		bool bLeft2 = GeomUtil::isRight(*ptEnd2,*ptStart1,*ptStart2);

		ArrowPoint*   res;
		if(isSameSide)//��ͬһ��
		{
			if(dis1 < dis2)
				res=ArrowPoint::interpolate(ptMid1,pt3,0.7);
			else
				res=ArrowPoint::interpolate(ptMid1,pt4,0.7);	

			delete ptMid1;delete ptMid2;delete vec;
			delete pt1;delete pt2;delete pt3; delete pt4;
			return res;
		}
		else//��ͬ��
		{
			ArrowPoint* pt5 = ArrowPoint::interpolate(ptMid1,ptMid2,0.9);
			double dis3 = ArrowPoint::distance(ptMid1,pt5);
			if(min(dis1,dis2) > dis3)
			{
				res= pt5;
			}
			else
			{
				if(dis1 < dis2)
					res= ArrowPoint::interpolate(pt3,ptMid1,0.3);
				else
					res= ArrowPoint::interpolate(pt4,ptMid1,0.3);
				delete pt5;
			}	
			delete ptMid1;delete ptMid2;delete vec;
			delete pt1;delete pt2;delete pt3; delete pt4;
			return res;
		}
	}


	std::vector<ArrowPoint*> SpecialGeomUtil::getLineArrowPts(ArrowPoint* ptStart,ArrowPoint* ptEnd,ArrowStyle* arrowStyle,double tail, double head, double ratio)
	{
		std::vector<ArrowPoint*> pts;
		//����β������
		ArrowPoint* fullVect = ptEnd->subtract(ptStart);
		ArrowPoint* startNorm = new ArrowPoint(fullVect->y,-fullVect->x);	
		double dis = ArrowPoint::distance(ptStart,ptEnd);
		startNorm->normalize(dis*tail);
		ArrowPoint* startLeft = ptStart->add(startNorm);
		ArrowPoint* startRight = ptStart->subtract(startNorm);			
		//�������
		pts.push_back(startLeft);
		//�����ͷͷ��
		ArrowPoint* ptArrowBegin = ArrowPoint::interpolate(ptStart,ptEnd,head);
		getArrowHeadPts(pts,ptArrowBegin,ptEnd,arrowStyle,ratio);
		//�����յ�
		pts.push_back(startRight);

		delete fullVect;
		delete startNorm;
		delete ptArrowBegin;
		return pts;
	}


	std::vector<ArrowPoint*> SpecialGeomUtil::getThreePointArrowPts(ArrowPoint* ptStart1,ArrowPoint* ptStart2,ArrowPoint* ptEnd,ArrowStyle* arrowStyle)
	{
		std::vector<ArrowPoint*> arrowPts ;
		
		ArrowPoint* ptMid = ArrowPoint::interpolate(ptStart1,ptStart2,0.5);//�е�
		
		ArrowPoint* ptLeft,*ptRight;
		//�ж�ptStart1��PtStart2��ptMid->ptEnd����໹���Ҳ࣬�Ա�����ȷ���ơ�
		if(GeomUtil::isRight(*ptStart1,*ptMid,*ptEnd))
		{
			ptLeft = ptStart2;
			ptRight = ptStart1;
		}
		else
		{
			ptLeft = ptStart1;
			ptRight = ptStart2;
		}
		
		ArrowPoint* ptStart = ArrowPoint::interpolate(ptMid,ptEnd,0.3);//��ͷ���
		
		ArrowHead* arrowHead = getArrowHead(ptStart,ptEnd,arrowStyle);//��ȡ��ͷ
		std::vector<ArrowPoint*> arrowHeadPts ;
		getArrowHeadPts(arrowHeadPts,ptStart,ptEnd,arrowStyle, 0.5);
		//��ȡ���Ƶ�
		ArrowPoint* ptCtrLeft = arrowHead->startLeft->add(arrowHead->startLeft->subtract(arrowHead->interLeft));
		ArrowPoint* ptCtrRight = arrowHead->startRight->add(arrowHead->startRight->subtract(arrowHead->interRight));
		
		//����
		GeomUtil::getQuadraticBezierPts(arrowPts,ptLeft,arrowHead->interLeft,ptCtrLeft,1);
		//arrowPts = arrowPts.concat(arrowHeadPts);
		//arrowPts.addAll(arrowHeadPts);
		arrowPts.insert(arrowPts.end(), arrowHeadPts.begin(), arrowHeadPts.end()); 
		std::vector<ArrowPoint*> rightPts ;
		GeomUtil::getQuadraticBezierPts(rightPts,arrowHead->interRight,ptRight,ptCtrRight,1);
		//arrowPts = arrowPts.concat(rightPts);
		//arrowPts.addAll(rightPts);
		arrowPts.insert(arrowPts.end(), rightPts.begin(), rightPts.end());

		delete ptCtrLeft;
		delete ptCtrRight;
		delete ptStart;
		delete arrowHead;
		delete ptMid;
		return arrowPts;
	}	

	double __NaN=9999999;
	std::vector<ArrowPoint*> SpecialGeomUtil::getCurveArrowPts(std::vector<ArrowPoint*> pts,ArrowStyle* arrowStyle,double ratio)
	{
		std::vector<ArrowPoint*> curveArrowPts ;
		int length = pts.size();
		
		//��ȡ����pts[length-2]�ı�������
		ArrowPoint* ptStart = (ArrowPoint*)pts[length-2];
		ArrowPoint* ptEnd = (ArrowPoint*)pts[length-1];
		ArrowPoint* tmppt= (ArrowPoint*)pts[length-3];
		BezierPoint* bezierPt = GeomUtil::getBezierPoint(tmppt,ptStart,ptEnd);			
		
		//��ȡ���޵ļ�ͷͷ��,��ͷ�ķ�����ȡbezierPt.leftCtrPoint->ptEnd����
		ArrowHead* arrowHead = getLimitedArrowHead(bezierPt->leftCtrPoint,ptEnd,0.5,200,*arrowStyle);	
		std::vector<ArrowPoint*> arrowHeadPts;
		getLimitedArrowHeadPts(arrowHeadPts,bezierPt->leftCtrPoint,ptEnd,0.5,200,arrowStyle);
		//����ֱ�ߵ�����αƽ���ߵ����飬����ݼ������
		double dis = ArrowPoint::distance(arrowHead->interLeft,arrowHead->interRight);
		std::vector<ArrowPoint*> leftPts ;
		std::vector<ArrowPoint*> rightPts ;
		double polylineLength = GeomUtil::getPolylineLength(pts);
		double beginLength = polylineLength*ratio;
		GeomUtil::getBogusParallelPts(pts,leftPts,rightPts,beginLength,dis/2,false, "JointStyle.MITER",__NaN);
		//leftPts[leftPts.length-1] = arrowHead.interLeft;
		//rightPts[rightPts.length-1] = arrowHead.interRight;
		ArrowPoint* ptLeft=(ArrowPoint*)leftPts[leftPts.size()-1];
		ArrowPoint* ptRight=(ArrowPoint*)rightPts[rightPts.size()-1];
		ptLeft->x=arrowHead->interLeft->x;ptLeft->y=arrowHead->interLeft->y;
		ptRight->x= arrowHead->interRight->x; ptRight->y= arrowHead->interRight->y;
				
				
		//�������ұ��������ߵ�����
		std::vector<ArrowPoint*> leftBezierPts = GeomUtil::getCurvePts(leftPts,false);
		std::vector<ArrowPoint*> rightBezierPts = GeomUtil::getCurvePts(rightPts,false);			
		
		leftPts.clear();
		rightPts.clear();
		//curveArrowPts = leftBezierPts.concat(arrowHeadPts);	
		//leftBezierPts.addAll(arrowHeadPts);
		leftBezierPts.insert(leftBezierPts.end(), arrowHeadPts.begin(), arrowHeadPts.end()); 
		curveArrowPts =leftBezierPts;
		//curveArrowPts = curveArrowPts.concat(rightBezierPts.reverse());
		//Collections.reverse(rightBezierPts);
		std::reverse(rightBezierPts.begin(),rightBezierPts.end());
		//curveArrowPts.addAll(rightBezierPts);
		curveArrowPts.insert(curveArrowPts.end(), rightBezierPts.begin(), rightBezierPts.end());

		delete arrowHead;
		delete bezierPt;
		return curveArrowPts;
	}

	std::vector<ArrowPoint*> SpecialGeomUtil::getHasTailCurveArrowPts(std::vector<ArrowPoint*> pts,ArrowStyle* arrowStyle)
	{
		int len = pts.size();
		std::vector<ArrowPoint*> arrowPts;
		
		ArrowPoint* pts0=(ArrowPoint*)pts[0];
		ArrowPoint* pts1=(ArrowPoint*)pts[1];
		ArrowPoint* pts2=(ArrowPoint*)pts[2];
		//Point ptMid= Point.interpolate(pts[0],pts[1],0.5);
		//Point ptTail = Point.interpolate(ptMid,pts[2],0.7);
		ArrowPoint* ptMid= ArrowPoint::interpolate(pts0,pts1,0.5);
		ArrowPoint* ptTail = ArrowPoint::interpolate(ptMid,pts2,0.7);
		
		//��ȡ�м������
		std::vector<ArrowPoint*> ptsMid ;
		ptsMid.push_back(ptMid);
		for(int i = 2;i < len; i++)
			ptsMid.push_back(pts[i]);
		
		//��ȡ����ptsMid[length-2]�ı�������
		int length = ptsMid.size();
		ArrowPoint* ptStart = (ArrowPoint*)ptsMid[length-2];
		ArrowPoint* ptEnd = (ArrowPoint*)ptsMid[length-1];
		BezierPoint* bezierPt= GeomUtil::getBezierPoint((ArrowPoint*)ptsMid[length-3],ptStart,ptEnd);			
		
		//��ȡ���޵ļ�ͷͷ��,��ͷ�ķ�����ȡbezierPt.leftCtrPoint->ptEnd����
		ArrowHead* arrowHead = getLimitedArrowHead(bezierPt->leftCtrPoint,ptEnd,0.5,100,*arrowStyle);	
		std::vector<ArrowPoint*> arrowHeadPts;
		getLimitedArrowHeadPts(arrowHeadPts,bezierPt->leftCtrPoint,ptEnd,0.5,100,arrowStyle);
		//����ֱ�ߵ�����αƽ���ߵ����飬����ݼ������
		double beginLength = ArrowPoint::distance(pts0,pts1)/4;
		double dis = ArrowPoint::distance(arrowHead->interLeft,arrowHead->interRight);
		std::vector<ArrowPoint*> leftPts ;
		std::vector<ArrowPoint*> rightPts ;
		GeomUtil::getBogusParallelPts(ptsMid,leftPts,rightPts,beginLength,dis/2,false,"JointStyle.MITER",__NaN);
		//if(GeomUtil.isRight((Point)pts[0],ptMid,pts[2]))
		if(GeomUtil::isRight(*pts0,*ptMid,*pts2))
		{
			//leftPts[0] = pts[1];
			//rightPts[0] = pts[0];
			ArrowPoint* leftpt0=(ArrowPoint*)leftPts[0];
			ArrowPoint* rightpt0=(ArrowPoint*)rightPts[0];
			leftpt0->set(pts[1]);
			rightpt0->set(pts[0]);
		}
		else
		{
			//leftPts[0] = pts[0];
			//rightPts[0] = pts[1];
			ArrowPoint* leftpt0=(ArrowPoint*)leftPts[0];
			ArrowPoint* rightpt0=(ArrowPoint*)rightPts[0];
			leftpt0->set(pts[0]);
			rightpt0->set(pts[1]);
		}
		
		//leftPts[length - 1] = arrowHead.interLeft;
		//rightPts[length - 1] = arrowHead.interRight;
		ArrowPoint* leftptlen_1=(ArrowPoint*)leftPts[length - 1];
		ArrowPoint* rightptlen_1=(ArrowPoint*)rightPts[length - 1];
		leftptlen_1->set(arrowHead->interLeft);
		rightptlen_1->set(arrowHead->interRight);
		
		//�������ұ��������ߵ�����
		std::vector<ArrowPoint*> leftBezierPts = GeomUtil::getCurvePts(leftPts,false);
		std::vector<ArrowPoint*> rightBezierPts = GeomUtil::getCurvePts(rightPts,false);
		
		//arrowPts = leftBezierPts.concat(arrowHeadPts);
		//leftBezierPts.addAll(arrowHeadPts);
		leftBezierPts.insert(leftBezierPts.end(), arrowHeadPts.begin(), arrowHeadPts.end());
		arrowPts=leftBezierPts;
		
		//arrowPts = arrowPts.concat(rightBezierPts.reverse());
		//Collections.reverse(rightBezierPts);
		std::reverse(rightBezierPts.begin(),rightBezierPts.end());
		//arrowPts.addAll(rightBezierPts);
		arrowPts.insert(arrowPts.end(), rightBezierPts.begin(), rightBezierPts.end());

		delete ptMid; delete ptTail;delete bezierPt;
		return arrowPts;
	}
