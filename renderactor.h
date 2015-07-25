#ifndef RENDERACTOR_H
#define RENDERACTOR_H

#include "actor.h"

class RenderActor : public Actor
{
public:
    RenderActor();

    virtual void init() = 0;
    virtual void render() = 0;
    virtual void update() = 0 ;
};

#endif // RENDERACTOR_H
