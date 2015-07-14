#ifndef CAMERA_H
#define CAMERA_H

#include "actor.h"

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <math.h>

#include "eigenutil.h"
#include "hmdinfo.h"



enum cameraProjection: int {
    Perspective = 0,
    Orthographic=1
};

enum HMDSpecies: int {
    Oculus = 0,
    Morpheus = 1
};

typedef struct widthheight{
    int width;
    int height;
}WidthHeight;

class Camera : public Actor
{
public:
    Camera();
    ~Camera();

    void setNormalProjection(cameraProjection state, WidthHeight &&wh);
    void setHMDProjection(HMDSpecies hmd);
   // void setProjection(cameraProjection state, WidthHeight wh);

    //*******important******//
    Eigen::Matrix4f getProjection();
    Eigen::Matrix4f MakelookAt();
    //*********************//

    Eigen::Matrix4f m_proj;



private:


    Eigen::Vector3f m_eye;
    Eigen::Vector3f m_center;
    Eigen::Vector3f m_eyeUp;
};

#endif // CAMERA_H
