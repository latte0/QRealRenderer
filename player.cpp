
#include "player.h"
#include "Kernel/OVR_Alg.h"





Player::Player()
{
    m_Position = QRRUtil::EigenVector3fMake(0.0f, 0.0f, 0.0f);
}

Player::~Player()
{
}

Eigen::Vector3f Player::getPosition()
{

}

Eigen::Vector3f Player::getEyeCenterPosition()
{
    return QRRUtil::EigenVector3fMake(m_Position.x(), m_height, m_Position.z());
}

Camera Player::getLeftEye(){
    return m_LeftEyeCam;
}

Camera Player::getRightEye(){
    return m_RightEyeCam;
}


