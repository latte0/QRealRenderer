
#ifndef PLAYER_H_
#define PLAYER_H_


#include <Eigen/Core>
#include <Eigen/Geometry>

#include "OVR_Kernel.h"
#include "Kernel/OVR_KeyCodes.h"
#include "Render/Render_Device.h"

#include "camera.h"
#include "eigenutil.h"


class Player
{
public:

    Player();
    ~Player();


    Eigen::Vector3f getPosition();
    Eigen::Vector3f getHeight();
    Eigen::Vector3f getEyeCenterPosition();


    Camera getLeftEye();
    Camera getRightEye();

private:

    float const m_height = 1700;
    float const m_eyeewight = 100;

    Camera m_LeftEyeCam;
    Camera m_RightEyeCam;

    Eigen::Vector3f m_Position;

};

#endif
