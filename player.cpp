
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
    m_Position = QRRUtil::EigenVector3fMake(0.0f, 0.0f, 400.0f /  QRR::Environment::mmDiv );

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

    Eigen::MatrixX4f cam_rot = QRRUtil::MakeMatrixfromQuat( orientation.x, orientation.y, orientation.z, orientation.w );


    std::cout << "cam_rot" << cam_rot << std::endl;

    Eigen::Vector3f s = QRRUtil::EigenVector3fMake(cam_rot(0,0),cam_rot(0,1),cam_rot(0,2));


    m_LeftEyeCam.setBasis(cam_rot);
    Eigen::Vector4f lefttemp = cam_rot * QRRUtil::EigenVector4fMake(-this->m_eyewidth / 2.0f, 0 ,0 , 1);
    auto xbasis = -this->m_eyewidth / 2.0f;

    lefttemp << s.x()*xbasis, s.y()*xbasis, s.z()*xbasis , 1;
    std::cout << lefttemp << std::endl;
    m_LeftEyeCam.setPosition( this->getPosition() +  QRRUtil::EigenVector3fMake(lefttemp.x(),lefttemp.y(),lefttemp.z()) );

    m_RightEyeCam.setBasis(cam_rot);
    Eigen::Vector4f righttemp = cam_rot * QRRUtil::EigenVector4fMake( this->m_eyewidth / 2.0f, 0 ,0 , 1);
    xbasis = -xbasis;
    righttemp << s.x()*xbasis, s.y()*xbasis, s.z()*xbasis , 1;
    std::cout << righttemp << std::endl;
    m_RightEyeCam.setPosition( this->getPosition() + QRRUtil::EigenVector3fMake(righttemp.x(),righttemp.y(),righttemp.z()) );

    std::cout << this->getPosition() << std::endl;

}

Eigen::Vector3f Player::getPosition()
{
    return this->m_Position;
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



