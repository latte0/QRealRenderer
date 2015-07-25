#ifndef RECTANGLEOBJECT_H
#define RECTANGLEOBJECT_H

#include "actor.h"
#include "boardobject.h"
#include "eigenutil.h"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>



class RectangleObject : BoardObject
{
    friend class WindowRenderer;
public:
    RectangleObject();
    ~RectangleObject();

    void initRecpos();
    void calcRecpos(Eigen::Matrix4f mat);


private:
    Eigen::Vector3f m_recPositions[4];

    Eigen::Vector3f m_rightVec;
    Eigen::Vector3f m_downVec;
    Eigen::Vector3f m_vertVec;

};

#endif // RECTANGLEOBJECT_H
