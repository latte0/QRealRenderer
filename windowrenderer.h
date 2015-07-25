
#ifndef WindowRenderer_H
#define WindowRenderer_H

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
#include "handinfo.h"
#include "rectangleobject.h"


QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)


class WindowRenderer : public Actor ,RectangleObject
{
    friend class qmlRenderer;

public:


    WindowRenderer();
    ~WindowRenderer();


    QMutex* m_mutex;


    void resize(int w, int h);
    void update(Eigen::Matrix4f mat, Eigen::Vector3f top, Eigen::Vector3f mousepos);
    void render(QOpenGLContext* share,Eigen::Matrix4f mat, Eigen::Vector3f top, Eigen::Vector3f mousepos);

    virtual void bindTex();
    virtual void inittex(QOpenGLContext* share);
    virtual void collide(Eigen::Vector3f top);


    virtual void init(QOpenGLContext* share, const QString &filename);
    void setupVertexAttribs(QOpenGLContext* share);

    void setCondition(float scale, Eigen::Vector3f pos, float uprot, float rightrot, bool handtouch);

    float calcV(Eigen::Vector3f top){
       m_v = m_rightVec.cross(m_downVec).dot(top - m_recPositions[0])/3.0;
    }



    Eigen::Vector2f calcPos(Eigen::Vector3f top){
        Eigen::Matrix3f mat;

        mat << m_rightVec.x(), m_downVec.x(), m_vertVec.x(),
                m_rightVec.y(), m_downVec.y(), m_vertVec.y(),
                m_rightVec.z(), m_downVec.z(), m_vertVec.z();

        Eigen::Vector3f ans;
        //ans = mat.inverse() * (top-m_positions[0]);
        ans = mat.inverse() * (top-m_recPositions[0]);
        Eigen::Vector2f xy;
        xy << ans.x() ,ans.y();
        return xy;
    }



    float getH(Eigen::Vector3f top){
        calcV(top);
        m_s = sqrt(m_rightVec.dot(m_rightVec)) * sqrt(m_downVec.dot(m_downVec));
        return m_v *3/m_s;
    }


    float m_z = -150.0;
    float m_s;
    float m_v;
    float m_scale = 70;
    float m_rightrot = 0;
    float m_uprot = 0;
    bool m_handtouch = false;
    bool m_touched = false;
    bool m_pressed = false;
    float touchdistance = 6.0;



private:

    QString m_filename;

    QOpenGLContext* m_context;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLBuffer m_indexbuffer;
    QOpenGLVertexArrayObject *m_vao;
    int m_matrixLoc;
    Eigen::Matrix4f m_proj;

    QMatrix4x4 m_qworld;
    QMatrix4x4 m_qmat;
    QMatrix4x4 m_smat;
};

#endif
