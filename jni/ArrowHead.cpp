#include "ArrowHead.h"

ArrowHead::ArrowHead(void)
{
}

ArrowHead::ArrowHead(ArrowPoint* startLeft_, ArrowPoint* startRight_, ArrowPoint* interLeft_, ArrowPoint* interRight_, ArrowPoint* edgeLeft_, ArrowPoint* edgeRight_, ArrowPoint* edgeCtrLeft_, ArrowPoint* edgeCtrRight_, ArrowPoint* end_)
{
    this->startLeft = startLeft_;
    this->startRight = startRight_;
    this->interLeft = interLeft_;
    this->interRight = interRight_;
    this->edgeLeft = edgeLeft_;
    this->edgeRight = edgeRight_;
    this->edgeCtrLeft = edgeCtrLeft_;
    this->edgeCtrRight = edgeCtrRight_;
    this->end = end_;
}
ArrowHead::~ArrowHead(void)
{
	
}
