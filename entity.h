#ifndef RENDERACTOR_H
#define RENDERACTOR_H

#include "actor.h"

class Entity : public Actor
{
public:
    Entity();
    virtual ~Entity();
/*
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void update() = 0 ;
    */
};

#endif // RENDERACTOR_H
