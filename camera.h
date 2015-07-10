#ifndef CAMERA_H
#define CAMERA_H

#include "actor.h"

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <math.h>

#include "eigenutil.h"


enum cameraProjection: int {
    Perspective = 0,
    Orthographic=1
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

    void setProjection(cameraProjection state, WidthHeight &&wh);
   // void setProjection(cameraProjection state, WidthHeight wh);
    void setCamPosition(Eigen::Vector3f pos);

    Eigen::Matrix4f getProjection();
    Eigen::Matrix4f getCamPosition();

    Eigen::Matrix4f m_proj;
    Eigen::Matrix4f m_camera;


private:


    Eigen::Vector3f m_eye;
    Eigen::Vector3f m_center;
    Eigen::Vector3f m_eyeUp;
};

#endif // CAMERA_H
