#ifndef BACKGROUNDRENDERER
#define BACKGROUNDRENDERER

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <QThread>
#include <thread>

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
#include "imagereceiver.h"

#include "cvutil.h"

#include "renderwindow.h"

class RenderWindow;


QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)


class BackGroundRenderer : Actor
{
    Q_OBJECT

public:
    BackGroundRenderer(int port, std::shared_ptr<QOpenGLContext>& share);
    virtual ~BackGroundRenderer();

    QMutex * m_mtx;

    void render();

    void init();
    void setupVertexAttribs();


private:

    std::shared_ptr<QOpenGLContext> m_context;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLVertexArrayObject *m_vao;
    std::unique_ptr<QOpenGLTexture> m_videotex;

    RenderWindow *m_rwindow;

    ImageReceiver *m_imgReceiver;
    QThread *imgthread;

    std::vector<uchar> video_ibuff;
    cv::Mat m_videoImage;

signals:
    void imgstart();

};

#endif // BACKGROUNDRENDERER

