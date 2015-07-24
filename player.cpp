
#include "player.h"
#include "environmentreader.h"

#include <QDebug>


Player::Player()
{
    PlayerInit();
    HmdInit();
}


Player::~Player()
{
}



void Player::PlayerInit()
{
    m_position = QRRUtil::EigenVector3fMake(0.0f, 0.0f, 400.0f /  QRR::Environment::mmDiv );

    m_LeftEyeCam.setHMDProjection(HMDSpecies::Oculus);
    m_RightEyeCam.setHMDProjection(HMDSpecies::Oculus);
}

static void OVR_CDECL PlayerHmdLog(int level, const char* message)
{
    OVR_UNUSED2(level, message);
}

void Player::HmdInit()
{
    ovrInitParams params = {0, 0, nullptr, 0};
    params.LogCallback = PlayerHmdLog;
    ovrBool result = ovr_Initialize(&params);

    ovr_TraceMessage(ovrLogLevel_Info, "oculus world dmo start up");

    if ( !result ) qDebug() << "oculusworlddemo", "unabel to initialize libovr";

    m_hmd = ovrHmd_Create(0);
       if (!m_hmd || !ovrHmd_ConfigureTracking(m_hmd,
           ovrTrackingCap_Orientation, 0))
       {
           qDebug() << "Unable to create hmd" << ovrHmd_GetLastError(NULL);
       }

       for (int i = 0; i < 10; ++i) {
         ovrTrackingState state = ovrHmd_GetTrackingState(m_hmd, 0);

         ovrQuatf orientation = state.HeadPose.ThePose.Orientation;
         qDebug () << orientation.x << orientation.y << orientation.z << orientation.w;

       }

}



void Player::update()
{
    ovrTrackingState state = ovrHmd_GetTrackingState(m_hmd, 0);

    ovrQuatf orientation = state.HeadPose.ThePose.Orientation;

    m_cam_rot = QRRUtil::MakeMatrixfromQuat( orientation.x, orientation.y, orientation.z, orientation.w );

    Eigen::Vector3f s = QRRUtil::EigenVector3fMake(m_cam_rot(0,0),m_cam_rot(0,1),m_cam_rot(0,2));


    m_LeftEyeCam.setBasis(m_cam_rot);
    Eigen::Vector4f lefttemp = m_cam_rot * QRRUtil::EigenVector4fMake(-this->m_eyewidth / 2.0f, 0 ,0 , 1);
    auto xbasis = -this->m_eyewidth / 2.0f;

    lefttemp << s.x()*xbasis, s.y()*xbasis, s.z()*xbasis , 1;

    m_LeftEyeCam.setPosition( this->getPosition() +  QRRUtil::EigenVector3fMake(lefttemp.x(),lefttemp.y(),lefttemp.z()) );

    m_RightEyeCam.setBasis(m_cam_rot);
    Eigen::Vector4f righttemp = m_cam_rot * QRRUtil::EigenVector4fMake( this->m_eyewidth / 2.0f, 0 ,0 , 1);
    xbasis = -xbasis;
    righttemp << s.x()*xbasis, s.y()*xbasis, s.z()*xbasis , 1;

    m_RightEyeCam.setPosition( this->getPosition() + QRRUtil::EigenVector3fMake(righttemp.x(),righttemp.y(),righttemp.z()) );

}

Eigen::Vector3f Player::getPosition()
{
    return this->m_position;
}

Eigen::Vector3f Player::getEyeCenterPosition()
{
    return QRRUtil::EigenVector3fMake(m_Position.x(), m_height, m_Position.z());
}

Eigen::Matrix4f Player::getLeftEyeMat(){
    std::cout << "left" << std::endl;
    std::cout << m_LeftEyeCam.MakelookAt() << std::endl;
    return m_LeftEyeCam.MakelookAt();
}

Eigen::Matrix4f Player::getRightEyeMat(){
    std::cout << "right" << std::endl;
    std::cout << m_RightEyeCam.MakelookAt() << std::endl;
    return m_RightEyeCam.MakelookAt();
}

void Player::toUp(){
    Eigen::Vector3f s = QRRUtil::EigenVector3fMake(m_cam_rot(2,0),0,m_cam_rot(2,2));
    m_position -= s * speed;
}

void Player::toDown(){
    Eigen::Vector3f s = QRRUtil::EigenVector3fMake(m_cam_rot(2,0),0,m_cam_rot(2,2));
    m_position += s * speed;
}

void Player::toRight(){
    Eigen::Vector3f s = QRRUtil::EigenVector3fMake(m_cam_rot(0,0),0,m_cam_rot(0,2));
    m_position -= s * speed;
}

void Player::toLeft(){
    Eigen::Vector3f s = QRRUtil::EigenVector3fMake(m_cam_rot(0,0),0,m_cam_rot(0,2));
    m_position += s * speed;
}

void Player::toOver(){
    m_position.y() += speed;
}

void Player::toBelow(){
    m_position.y() -= speed;
}



