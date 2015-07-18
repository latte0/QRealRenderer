#ifndef MOUSERENDERER_H
#define MOUSERENDERER_H

#include <Eigen/Core>
#include <Eigen/Dense>
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
#include <QMutex>

#include "renderwindow.h"
#include "actor.h"
#include "handinfo.h"

#include "eigenutil.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)


class MouseRenderer : public Actor
{
public:
    MouseRenderer();
    ~MouseRenderer();


    void resize(int w, int h);
    void update(Eigen::Matrix4f mat);
    void render(QOpenGLContext* share,Eigen::Matrix4f mat);

    void init(QOpenGLContext* share);
    void setupVertexAttribs(QOpenGLContext* share);


    float m_z = -149.99;
    float m_scale = 3;
    float m_rightrot = 0;
    float m_uprot = 0;
    Eigen::Vector3f m_rightVec;
    Eigen::Vector3f m_downVec;
    Eigen::Vector3f m_suiVec;

    Eigen::Vector3f m_positions[4];
    Eigen::Vector3f m_centerpos;


private:

    QOpenGLContext* m_context;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLBuffer m_indexbuffer;
    QOpenGLVertexArrayObject *m_vao;
    int m_matrixLoc;
    Eigen::Matrix4f m_proj;
};

#endif // MOUSERENDERER_H
