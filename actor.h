#ifndef ACTOR_H
#define ACTOR_H

#include <QDebug>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "eigenutil.h"

enum ActorSpecies: int{
    FBX = 0,
    LINE = 1,
    BACK = 2,
    QML = 3,
    HAND = 4,
    CAMERA = 5
};

class Actor
{
public:
    Actor();
    ~Actor();

    Eigen::Vector3f m_position;
    Eigen::Matrix4f m_basis;

    const Eigen::Vector3f getPosition(){
        return m_position;
    }

    const Eigen::Matrix4f getBasis(){
        return m_basis;
    }




};

#endif // ACTOR_H
