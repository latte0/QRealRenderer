#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <map>
#include <iostream>
#include <vector>

#include "actor.h"
#include "scenerendermodel.h"
#include "camera.h"
#include "light.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void render();

    void addlight();
    void addmodel();
    void addcamera();

private:

    std::vector<Light> lights;
    std::vector<SceneRenderModel> rendermodels;
    std::vector<Camera> camera;


};

#endif // SCENE_H
