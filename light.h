#ifndef LIGHT_H
#define LIGHT_H

#include "actor.h"

#include "iostream"

class Light : public Actor
{
public:
    Light();
    ~Light();
};

class SpotLight : Light
{

};

class DirectLight : Light
{

};

class PointLight : Light
{

};

#endif // LIGHT_H
