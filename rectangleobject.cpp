#include "rectangleobject.h"

RectangleObject::RectangleObject()
{

    m_recPositions[0] = QRRUtil::EigenVector3fMake(-1.0, 1.0, 0.0);
    m_recPositions[1] = QRRUtil::EigenVector3fMake(1.0, 1.0, 0.0);
    m_recPositions[2] = QRRUtil::EigenVector3fMake(-1.0, -1.0, 0.0);
    m_recPositions[3] = QRRUtil::EigenVector3fMake(1.0, -1.0, 0.0);

}


RectangleObject::~RectangleObject()
{

}

void RectangleObject::initRecpos(){
    m_recPositions[0] = QRRUtil::EigenVector3fMake(-1.0, 1.0, 0.0);
    m_recPositions[1] = QRRUtil::EigenVector3fMake(1.0, 1.0, 0.0);
    m_recPositions[2] = QRRUtil::EigenVector3fMake(-1.0, -1.0, 0.0);
    m_recPositions[3] = QRRUtil::EigenVector3fMake(1.0, -1.0, 0.0);
}

void RectangleObject::calcRecpos(Eigen::Matrix4f mat){
    Eigen::Vector4f positions[4];

    positions[0] = QRRUtil::EigenVector4fMake(-1.0, 1.0, 0.0,1.0);
    positions[1] = QRRUtil::EigenVector4fMake(1.0, 1.0, 0.0,1.0);
    positions[2] = QRRUtil::EigenVector4fMake(-1.0, -1.0, 0.0,1.0);
    positions[3] = QRRUtil::EigenVector4fMake(1.0, -1.0, 0.0,1.0);

    for(int i =0;i < 4;i++){
        positions[i] = mat * positions[i];
        m_recPositions[i] = QRRUtil::EigenVector3fMake(positions[i].x(),positions[i].y(),positions[i].z());
    }

}
