
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
#include "entity.h"
#include "handinfo.h"
#include "rectangleobject.h"


QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)


class WindowRenderer : public Entity ,public RectangleObject
{
    friend class qmlRenderer;
    friend class MouseRenderer;

public:


    WindowRenderer(std::shared_ptr<QOpenGLContext>& share);
    virtual ~WindowRenderer();


    QMutex* m_mutex;


    void resize(int w, int h);
    void objectupdate(Eigen::Matrix4f mat, Eigen::Vector3f top);
    void render(Eigen::Matrix4f mat, Eigen::Vector3f top);

    virtual void bindTex();
    virtual void inittex();
    virtual void collide(Eigen::Vector3f top);
    virtual void update();

    //????
    void init(const QString &filename);


    void setupVertexAttribs();

    void setCondition(float scale, Eigen::Vector3f pos, float uprot, float rightrot, bool handtouch);

    inline float calcV(Eigen::Vector3f top){
       m_v = m_rightVec.cross(m_downVec).dot(top - m_recPositions[0])/3.0;
    }



    inline Eigen::Vector2f calcPos(Eigen::Vector3f top){
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



    inline float getH(Eigen::Vector3f top){
        calcV(top);
        m_s = sqrt(m_rightVec.dot(m_rightVec)) * sqrt(m_downVec.dot(m_downVec));
        return m_v *3/m_s;
    }





protected:

    QString m_filename;

    std::shared_ptr<QOpenGLContext> m_context;

    QWindow* m_window;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLBuffer m_indexbuffer;
    QOpenGLVertexArrayObject *m_vao;
    int m_matrixLoc;
    int m_aspectLoc;
    Eigen::Matrix4f m_proj;

    QMatrix4x4 m_qworld;
    QMatrix4x4 m_qmat;
    QMatrix4x4 m_smat;

    float m_z;
    float m_s;
    float m_v;
    float m_scale;
    float m_rightrot;
    float m_uprot;
    bool m_handtouch ;
    bool m_touched;
    bool m_pressed;
    float touchdistance;


};

#endif
