#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}


void Camera::setNormalProjection(cameraProjection state, WidthHeight &&wh)
{
    switch(state){
        case Perspective:
            m_proj = QRRUtil::perspective(45.0f, (float)wh.width/ wh.height, 0.1f, 10000.0f);
            break;
        case Orthographic:
            break;
    }
}

void Camera::setHMDProjection(HMDSpecies hmd)
{
    switch(hmd){
        case Oculus:
            m_proj = QRRUtil::perspective(OculusInfo::eyeProjection, OculusInfo::eyeAspect, 0.1f, 10000.0f);
            break;
        case Morpheus:
             m_proj = QRRUtil::perspective(MorpheusInfo::eyeProjection, MorpheusInfo::eyeAspect, 0.1f, 10000.0f);
            break;
    }
}



Eigen::Matrix4f Camera::MakelookAt(){

    Eigen::Matrix4f res;

    Eigen::Vector3f f = QRRUtil::EigenVector3fMake(m_basis(2,0),m_basis(2,1),m_basis(2,2));
    Eigen::Vector3f u = QRRUtil::EigenVector3fMake(m_basis(1,0),m_basis(1,1),m_basis(1,2));
    Eigen::Vector3f s = QRRUtil::EigenVector3fMake(m_basis(0,0),m_basis(0,1),m_basis(0,2));

    res << s.x(), s.y(), s.z(), s.dot(getPosition()),
           u.x(), u.y(), u.z(), u.dot(getPosition()),
           f.x(), f.y(), f.z(), f.dot(getPosition()),
           0    , 0    , 0    ,  1         ;

    return res;
}

Eigen::Matrix4f Camera::getProjection()
{
    return m_proj;
}


