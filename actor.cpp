#include "actor.h"

Actor::Actor()
{
    id = rd();
    m_position = Eigen::Vector3f::Zero(3);
    m_basis = Eigen::Matrix4f::Zero(4,4);
}

Actor::~Actor()
{

}


