#ifndef HANDINFO_H
#define HANDINFO_H

#include "external/Leap.h"
#include "eigenutil.h"

struct FingerData
{
    Eigen::Vector3f direction;
    Eigen::Vector3f position;
    Eigen::Matrix4f direction_mat;
    Eigen::Matrix4f position_mat;
};

class HandInfo
{
public:
    HandInfo();
    ~HandInfo();

    float m_PointDiv;

    bool m_righthand;

    bool UpdateInfo();
    Eigen::Matrix4f LeaptoEigenmat(Leap::Matrix lmat);

    Eigen::Vector3f m_handGlobalPos;
    Eigen::Matrix4f m_handGlobalNorm;
    Eigen::Vector3f m_handGlobalDirect;

    float m_pitch,m_yaw,m_roll;

    FingerData m_fingerdata[5][4];


private:
    Leap::Controller m_Leap;

    const std::string fingerNames[5] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
    const std::string boneNames[4] = {"Metacarpal", "Proximal", "Middle", "Distal"};
    const std::string stateNames[4] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};



};

#endif // HandInfo_H
