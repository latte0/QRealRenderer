#include "rectangleobject.h"

RectangleObject::RectangleObject()
{

    m_positions[0] = QRRUtil::EigenVector3fMake(-1.0, 1.0, 0.0);
    m_positions[1] = QRRUtil::EigenVector3fMake(1.0, 1.0, 0.0);
    m_positions[2] = QRRUtil::EigenVector3fMake(-1.0, -1.0, 0.0);
    m_positions[3] = QRRUtil::EigenVector3fMake(1.0, -1.0, 0.0);
    //m_centerpos = QRRUtil::EigenVector3fMake(0.0, 0.0, 0.0);

}


RectangleObject::~RectangleObject()
{

}

