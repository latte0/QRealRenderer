
#ifndef OVR_WorldDemo_Player_h
#define OVR_WorldDemo_Player_h

#include "OVR_Kernel.h"
#include "Kernel/OVR_KeyCodes.h"
#include "Render/Render_Device.h"

using namespace OVR;
using namespace OVR::Render;


const OVR::Vector3f	RightVector(1.0f, 0.0f, 0.0f);
const OVR::Vector3f	UpVector(0.0f, 1.0f, 0.0f);
const OVR::Vector3f	ForwardVector(0.0f, 0.0f, -1.0f); // -1 because HMD looks along -Z at identity orientation

const float		YawInitial	= 0.0f;
const float		Sensitivity	= 0.3f; // low sensitivity to ease people into it gently.
const float		MoveSpeed	= 3.0f; // m/s

// These are used for collision detection
const float		RailHeight	= 0.8f;


//-------------------------------------------------------------------------------------
// ***** Player

// Player class describes position and movement state of the player in the 3D world.
class Player
{
public:
    // Where the avatar head is positioned and oriented in the virtual world
    OVR::Vector3f            GetPosition();
    Quatf               GetOrientation(bool baseOnly = false);

    // Returns virtual world position based on a real world head pose.
    // Allows predicting eyes separately based on scanout time.
    Posef               VirtualWorldTransformfromRealPose(const Posef &sensorHeadPose);

    // Handle directional movement. Returns 'true' if movement was processed.
    bool                HandleMoveKey(OVR::KeyCode key, bool down);

    void                HandleMovement(double dt, OVR::Array<Ptr<CollisionModel> >* collisionModels,
                                       OVR::Array<Ptr<CollisionModel> >* groundCollisionModels, bool shiftDown);

    float               GetScaledEyeHeight() { return UserEyeHeight * HeightScale; }

    Player();
    ~Player();

public:
    // User parameters
    float       UserEyeHeight;
    float       HeightScale;

    // Where the avatar coordinate system (and body) is positioned and oriented in the virtual world
    // Modified by gamepad/mouse input
    OVR::Vector3f    BodyPos;
    Anglef      BodyYaw;

    // Where the player head is positioned and oriented in the real world
    Posef       HeadPose;

    // Movement state; different bits may be set based on the state of keys.
    uint8_t     MoveForward;
    uint8_t     MoveBack;
    uint8_t     MoveLeft;
    uint8_t     MoveRight;
    OVR::Vector3f    GamepadMove, GamepadRotate;
    bool        bMotionRelativeToBody;
};

#endif
