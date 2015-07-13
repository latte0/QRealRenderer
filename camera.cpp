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

void Camera::setPosition(Eigen::Vector3f pos)
{

}

void Camera::setAxis(Eigen::Matrix4f axis)
{

}

Eigen::Matrix4f MakelookAt(){

    Eigen::Matrix4f res;

    return res;
}

Eigen::Matrix4f Camera::getProjection()
{
    return m_proj;
}

Eigen::Vector3f Camera::getPosition()
{
    return m_pos;
}
