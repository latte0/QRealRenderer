#ifndef RECTANGLEOBJECT_H
#define RECTANGLEOBJECT_H

#include "actor.h"
#include "boardobject.h"

class RectangleObject : public Actor , BoardObject
{
public:
    RectangleObject();
    ~RectangleObject();

    void update(Eigen::Matrix4f mat, Eigen::Vector3f top, Eigen::Vector3f mousepos);
/*
    virtual void bindTex();
    virtual void inittex();
    virtual void collide(Eigen::Vector3f top);
*/
private:
    Eigen::Vector3f m_positions[4];


    float m_z = -150.0;
    float m_s;
    float m_v;
    float m_scale = 70;
    float m_rightrot = 0;
    float m_uprot = 0;

    bool m_handtouch = false;
    bool m_touched = false;
    bool m_pressed = false;
    float touchdistance = 6.0;


};

#endif // RECTANGLEOBJECT_H
