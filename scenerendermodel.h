#ifndef SCENEMODEL_H
#define SCENEMODEL_H

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Compatibility>

#include "actor.h"
#include "scenerendermodel.h"
#include "shaderobject.h"
#include "vertexbufferobject.h"
#include "camera.h"
//#include "vertexarrayobject.h"

#include <QDebug>


typedef struct modelupdate{
    std::string model_filename;
    std::string sh_filename;
    Camera cam;
}ModelUpdate;


class SceneRenderModel : public Actor,  protected QOpenGLFunctions_3_3_Core
{
public:
    SceneRenderModel();
    ~SceneRenderModel();

    Eigen::Matrix4f m_stateMat;

    Eigen::Matrix4f getStateMat();
    Eigen::Matrix4f getTransMat();
    Eigen::Matrix4f getRotationMat();
/*
    ShaderObject m_shaderobj;
    VertexBufferObject m_vertbufobj;
*/
    virtual void render() = 0;
    virtual void update(ModelUpdate&&) = 0;
    virtual void set() = 0;


private:

    int i;
};

#endif // SCENEMODEL_H
