#ifndef BACKGROUNDRENDERER
#define BACKGROUNDRENDERER

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

#include "actor.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)


class BackGroundRenderer : Actor
{
public:
    BackGroundRenderer();
    ~BackGroundRenderer();

    QMutex* m_mutex;

    void resize(int w, int h);
    void render(QOpenGLContext* share);

    void init(QOpenGLContext* share);
    void setupVertexAttribs(QOpenGLContext* share);

    Eigen::Vector3f m_positions[4];

    float m_scale = 10;


private:
    QOpenGLContext* m_context;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLVertexArrayObject *m_vao;

};

#endif // BACKGROUNDRENDERER

