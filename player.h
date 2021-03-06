
#ifndef PLAYER_H_
#define PLAYER_H_


#include <Eigen/Core>
#include <Eigen/Geometry>


#include "OVR.h"
#include "OVR_Kernel.h"
#include "Kernel/OVR_Types.h"
#include "Kernel/OVR_Allocator.h"
#include "Kernel/OVR_RefCount.h"
#include "Kernel/OVR_Log.h"
#include "Kernel/OVR_System.h"
#include "Kernel/OVR_Nullptr.h"
#include "Kernel/OVR_String.h"
#include "Kernel/OVR_Array.h"
#include "Kernel/OVR_Timer.h"
#include "Kernel/OVR_DebugHelp.h"
#include "Extras/OVR_Math.h"
#include "Render/Render_Device.h"
#include "OVR_CAPI.h"

#include "external/external_opengl/FreeImage/FreeImage.h"
#include "external/cppitertools/itertools.hpp"
#include "external/external_opengl/glm/glm.hpp"


#include "camera.h"
#include "eigenutil.h"
#include "singleton.h"
#include "environmentreader.h"

/****************
 * the player only have hmd devices
 * ***************/


class Player : public Actor, public QRR::Singleton<Player>
{

public:
    friend class QRR::Singleton<Player>;

    Player();
    ~Player();

    void PlayerInit();
    void HmdInit();
    void update();

    float speed = 1;

    bool singleEye = false;

    Eigen::Vector3f getPosition();
    Eigen::Vector3f getHeight();
    Eigen::Vector3f getEyeCenterPosition();


    Eigen::Matrix4f getLeftEyeMat();
    Eigen::Matrix4f getRightEyeMat();
    Eigen::Matrix4f getEyeMat(QRR::EyeSide eye);

    void toUp();
    void toDown();
    void toRight();
    void toLeft();
    void toOver();
    void toBelow();

    Camera m_LeftEyeCam;
    Camera m_RightEyeCam;


private:

    Eigen::MatrixX4f m_cam_rot;
    float const m_height = 1700;
    float const m_eyewidth = 3;


    Eigen::Vector3f m_Position;

    /* oculus */
    bool isHmdInitialized = false;
    ovrHmd m_hmd;
    /**********/

};

#endif
