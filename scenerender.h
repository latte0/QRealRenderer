
#ifndef GAMEAPPLICATION_H
#define GAMEAPPLICATION_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFramebufferObject>
#include <QPoint>
#include <QMouseEvent>
#include <QTimer>
#include <QFile>
#include <QUdpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QMessageBox>

#include <QOffscreenSurface>
#include <QScreen>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickRenderControl>

#include <QWindow>
#include <QMatrix4x4>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>


#include <math.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <highgui.hpp>
#include <core.hpp>
#include <imgproc.hpp>


//http://memotiyou.blogspot.jp/2012/03/qt-c-qudpsocket_37.html

#include "external/external_opengl/FreeImage/FreeImage.h"
#include "external/cppitertools/itertools.hpp"
#include "external/external_opengl/glm/glm.hpp"

#include "glutil.h"

#include "handinfo.h"
#include "camera.h"
#include "rendertexture.h"
#include "shaderobject.h"
#include "vertexbufferobject.h"
#include "player.h"
#include "windowrenderer.h"
#include "renderwindow.h"
#include "backgroundrenderer.h"
#include "mouserenderer.h"
#include "ovrcondition.h"
#include "fbxrenderer.h"
#include "handfbxrenderer.h"
#include "qmlrenderer.h"
#include "copywindow.h"
#include "singleton.h"
#include "scene.h"

//http://stackoverflow.com/questions/17420739/opencv-2-4-5-and-qt5-error-s

#include "LinearMath/btAlignedObjectArray.h"

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;




class RenderWindow;
class qmlRenderer;
class WindowRenderer;
class MouseRenderer;

class BackGroundRenderer;


using namespace Eigen;
using namespace QRRUtil;

QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLFramebufferObject)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)


class SceneRender : public QWindow
{
    Q_OBJECT

public:
    SceneRender();
    ~SceneRender();

    qmlRenderer *cube;
    qmlRenderer *kyou;
    qmlRenderer *currentQml;

    BackGroundRenderer *back;
    handFbxRenderer *handfbxrender;
    MouseRenderer *mouse;
    bool touched = false;

    CopyWindow *copywindow;
    QMutex* scenemutex;
    QImage sceneimage;

    int debugmode = 0;
    OVRCondition m_ovrsender;
    Eigen::Vector3f m_position;

    bool m_initialized = false;




    void initFB();
    void init();
    void paint();


    unsigned int m_distortsampler;


    QOpenGLFramebufferObject* m_rightEyeTex;
    QOpenGLFramebufferObject* m_leftEyeTex;
    QOpenGLFramebufferObject* composeEyeTex;
    QOpenGLFramebufferObject* lastcomposeEyeTex;
    QOpenGLFramebufferObject* finalTexture;

    QOpenGLContext *m_context;
    QOffscreenSurface *m_offscreenSurface;
    QTimer m_updateTimer;


protected:
    void exposeEvent(QExposeEvent *e);
    void resizeEvent(QResizeEvent *e);

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;

    void updateuniform(QRR::EyeSide eye) ;

private:

    Eigen::Vector3f eye;
    Eigen::Vector3f center;
    Eigen::Vector3f eyeUp;

    Eigen::Matrix4f m_eproj;
    Eigen::Matrix4f m_ecamera;
    Eigen::Matrix4f m_eworld;

	bool m_transparent;

    int m_frame = 0;

    GLuint m_uniformBufferVs;
    UniformVs m_uniformVs;


    QOpenGLShaderProgram *distort_program;



    unsigned int m_quadpos_index;
    unsigned int m_quadpos_array;
    unsigned int m_quadtex_coord_index;
    unsigned int m_quadtex_coord_array;
    unsigned int m_quadvertex_arrays;
    unsigned int m_quadindex_array;

    unsigned int m_quadtexture;
    unsigned int m_quadsampler;

    const float m_quadpositions_rotate[12] = {
                          -1.0f,  1.0f, 0.0f,
                           1.0f, 1.0f, 0.0f,
                          -1.0f,  -1.0f, 0.0f,
                           1.0f, -1.0f, 0.0f,
                         };


    const float m_quadpositions[12] = { 1.0f, -1.0f, 0.0f,
                          1.0f,  1.0f, 0.0f,
                         -1.0f, -1.0f, 0.0f,
                         -1.0f,  1.0f, 0.0f};

    const float m_quadtex_coords[8] = {1.0f, 0.0f,
                          1.0f, 1.0f,
                          0.0f, 0.0f,
                          0.0f, 1.0f};

    const unsigned int m_quadindices[4] = {0,1,2,3};



private slots:
    void processing();

};

#endif // GAMEAPPLICATION_H
