#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}


void Camera::setProjection(cameraProjection state, WidthHeight &&wh)
{
    switch(state){
    case Perspective:
        m_proj = QRRUtil::perspective(45.0f, (float)wh.width/ wh.height, 0.1f, 10000.0f);
        break;
    case Orthographic:
        break;
    }

}

void Camera::setCamPosition(Eigen::Vector3f pos)
{
    auto position = QRRUtil::EigenVector3fMake(0.0f, 10.0f, 60.0f);

    Eigen::Quaternionf quat;
    Eigen::Vector3f axis;
    axis<<0,0,1;
    quat=Eigen::AngleAxisf(M_PI * (0 / 100.0),axis);

    m_eye = position;
    m_center= QRRUtil::EigenVector3fMake(0.0f, 0.0f, 0.0f);
    m_eyeUp= QRRUtil::EigenVector3fMake(0.0f, 1.0f , 0.0f);


    m_camera = QRRUtil::lookAt(m_eye, m_center, m_eyeUp);
}

Eigen::Matrix4f Camera::getProjection()
{
    return m_proj;
}

Eigen::Matrix4f Camera::getCamPosition()
{
    return m_camera;
}
