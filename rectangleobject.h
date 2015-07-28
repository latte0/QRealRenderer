#ifndef RECTANGLEOBJECT_H
#define RECTANGLEOBJECT_H

#include "actor.h"
#include "boardobject.h"
#include "eigenutil.h"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>



class RectangleObject : public BoardObject
{
    friend class WindowRenderer;
    friend class qmlRenderer;
    friend class MouseRenderer;

public:
    RectangleObject();
    virtual ~RectangleObject();

    void initRecpos();
    void calcRecpos(Eigen::Matrix4f mat);

    void setAspect(float aspect);
    const float getAspect() const;

    const Eigen::Vector3f getRightVec() const;

    const Eigen::Vector3f getDownVec() const;

    const Eigen::Vector3f getVertVec() const;


protected:
    Eigen::Vector3f m_recPositions[4];

    Eigen::Vector3f m_rightVec;
    Eigen::Vector3f m_downVec;
    Eigen::Vector3f m_vertVec;

    float m_aspect;
};

#endif // RECTANGLEOBJECT_H
