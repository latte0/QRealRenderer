#ifndef ACTOR_H
#define ACTOR_H

#include <QObject>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <random>

#include <list>
#include <map>
#include <iostream>
#include <memory>
#include <vector>

#include "eigenutil.h"



enum ActorSpecies: int{
    FBX = 0,
    LINE = 1,
    BACK = 2,
    QML = 3,
    HAND = 4,
    CAMERA = 5
};

class Actor :public QObject
{
public:

    Actor();
    virtual ~Actor();

    using actorPtr = std::shared_ptr<Actor>;

    void setPosition(Eigen::Vector3f pos){
        m_position = pos;
    }

    void setBasis(Eigen::Matrix4f basis){
        m_basis = basis;
    }

    void setWorld(Eigen::Matrix4f world){
        m_world = world;
    }

    void createWorld(){
        m_world << m_basis(0,0) , m_basis(0,1) , m_basis(0,2) , m_position.x(),
                   m_basis(1,0) , m_basis(1,1) , m_basis(1,2) , m_position.y(),
                   m_basis(2,0) , m_basis(2,1) , m_basis(2,2) , m_position.z(),
                   0            ,           0  ,            0 , 1;

    }

    const Eigen::Vector3f getPosition(){
        return m_position;
    }

    const Eigen::Matrix4f getBasis(){
        return m_basis;
    }

    const Eigen::Matrix4f getWorld(){
        return m_world;
    }

    bool operator <(const Actor& ractor){
        return this->id < ractor.id;
    }

    int getId(){ return id; }

    actorPtr getParent(){
        return ActorParent;
    }

    std::list<actorPtr> getChildren(){
        return ActorChildren;
    }


    Eigen::Vector3f m_position;
    Eigen::Matrix4f m_basis;
    Eigen::Matrix4f m_world;


private:


    actorPtr ActorParent;
    std::list<actorPtr> ActorChildren;


    int id;
    std::random_device rd;

};

#endif // ACTOR_H
