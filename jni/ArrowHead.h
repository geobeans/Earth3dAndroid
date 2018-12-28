#ifndef __ARROWHEAD_H__
#define __ARROWHEAD_H__
class ArrowPoint;
class ArrowHead
{
public:
	ArrowHead(void);
	~ArrowHead(void);

	 ArrowHead(ArrowPoint* startLeft_, ArrowPoint* startRight_, ArrowPoint* interLeft_, ArrowPoint* interRight_, ArrowPoint* edgeLeft_, ArrowPoint* edgeRight_, ArrowPoint* edgeCtrLeft_, ArrowPoint* edgeCtrRight_, ArrowPoint* end_);

	ArrowPoint* startLeft ;
    ArrowPoint* startRight ;
    ArrowPoint* interLeft ;
    ArrowPoint* interRight ;
    ArrowPoint* edgeLeft ;
    ArrowPoint* edgeRight ;
    ArrowPoint* edgeCtrLeft ;
    ArrowPoint* edgeCtrRight ;
    ArrowPoint* end ;
};

#endif
