
#ifndef CUBERENDERER_H
#define CUBERENDERER_H

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

class RenderWindow;

QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)


class CubeRenderer : Actor
{
public:
    CubeRenderer();
    ~CubeRenderer();

    RenderWindow *m_rwindow;
    QMutex* m_mutex;
    QOpenGLTexture *m_qmltex;

    void resize(int w, int h);
    void render(QOpenGLContext* share,Eigen::Matrix4f mat, Eigen::Vector3f top);

    void init(QOpenGLContext* share);
    void setupVertexAttribs(QOpenGLContext* share);

    float calcV(Eigen::Vector3f top){

       m_v = m_rightVec.cross(m_downVec).dot(top - m_positions[0])/3.0;

       std::cout << "volume " << m_v << std::endl;
    }

    Eigen::Vector2f calcPos(Eigen::Vector3f top){
        Eigen::Matrix3f mat;

        mat << m_rightVec.x(), m_downVec.x(), m_suiVec.x(),
                m_rightVec.y(), m_downVec.y(), m_suiVec.y(),
                m_rightVec.z(), m_downVec.z(), m_suiVec.z();



        Eigen::Vector3f ans;
        ans = mat.inverse() * (top-m_positions[0]);
        Eigen::Vector2f xy;
        xy << ans.x() ,ans.y();
                std::cout << "top" << top << "houteishikinokai" << mat << "zahyou" << xy << std::endl;
        return xy;
    }

    float getH(Eigen::Vector3f top){
        calcV(top);
        m_s = sqrt(m_rightVec.dot(m_rightVec)) * sqrt(m_downVec.dot(m_downVec));
        return m_v *3/m_s;
    }

    Eigen::Vector3f m_positions[4];

    float m_z = -1.0;
    float m_s;
    float m_v;
    float m_scale = 10;
    Eigen::Vector3f m_rightVec;
    Eigen::Vector3f m_downVec;
    Eigen::Vector3f m_suiVec;

private:



    QOpenGLContext* m_context;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLBuffer m_indexbuffer;
    QOpenGLVertexArrayObject *m_vao;
    int m_matrixLoc;
    Eigen::Matrix4f m_proj;
};

#endif
