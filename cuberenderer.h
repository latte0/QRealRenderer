
#ifndef CUBERENDERER_H
#define CUBERENDERER_H

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFramebufferObject>

#include <QOffscreenSurface>
#include <QOpenGLBuffer>

#include "actor.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)


class CubeRenderer : Actor
{
public:
    CubeRenderer();
    ~CubeRenderer();





    void resize(int w, int h);
    void render(QOpenGLContext* share,Eigen::Matrix4f mat, QOpenGLTexture *qmltex);

    void init(QOpenGLContext* share);
    void setupVertexAttribs(QOpenGLContext* share);

    float getS(){

    }

    float getH(){

    }



private:

    float m_s;
    float m_scale;
    float m_leftup;
    float m_rightup;
    float m_leftdown;
    float m_rightVec;
    float m_downVec;

    QOpenGLContext* m_context;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLVertexArrayObject *m_vao;
    int m_matrixLoc;
    Eigen::Matrix4f m_proj;
};

#endif
