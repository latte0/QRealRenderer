
#include "player.h"


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
    m_Position = QRRUtil::EigenVector3fMake(50.0f, 30.0f, 20.0f);

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

    m_LeftEyeCam.setBasis(cam_rot);
    Eigen::Vector4f lefttemp = cam_rot * QRRUtil::EigenVector4fMake(this->m_eyewidth / 2.0f, 0 ,0 , 0);
    m_LeftEyeCam.setPosition( this->getPosition() -  QRRUtil::EigenVector3fMake(lefttemp.x(),lefttemp.y(),lefttemp.z()) );

    m_RightEyeCam.setBasis(cam_rot);
    Eigen::Vector4f righttemp = cam_rot * QRRUtil::EigenVector4fMake(this->m_eyewidth / 2.0f, 0 ,0 , 0);
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
    return m_LeftEyeCam.MakelookAt();
}

Eigen::Matrix4f Player::getRightEyeMat(){
    return m_RightEyeCam.MakelookAt();
}



