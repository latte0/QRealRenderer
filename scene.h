#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <map>
#include <iostream>
#include <vector>


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
#include "light.h"

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
class FbxRenderer;
class WindowRenderer;
class MouseRenderer;
class BackGroundRenderer;
class SceneRender;

enum SceneData : short{
    TOML,
    XML
};

enum ModelData : short{
  FBX_MODEL,
    OBJ_MODEL
};

struct QmlObject{
    float scale;
    Eigen::Vector3f pos;
    float euler_x;
    float euler_y;
    bool touched;
    QString filename;
};



using  actorPtr = std::shared_ptr<Actor>;
using  lightPtr = std::shared_ptr<Light>;
using  fbxrendererPtr = std::shared_ptr<FbxRenderer>;
using  qmlrendererPtr = std::shared_ptr<qmlRenderer>;

class Scene
{
public:

    Scene(std::shared_ptr<QOpenGLContext> context);
    ~Scene();

    std::shared_ptr<QOpenGLContext> m_context;


    void addLight();
    void addModel(const ModelData div, const QString filename);
    void addQml(const QmlObject&& qmlobj);
    void addMouse();

    void addActor(const actorPtr actorobj);

    template <class T>
    std::list<std::shared_ptr<T>> getActors();


    void update();


    Eigen::Vector3f eye;
    Eigen::Vector3f center;
    Eigen::Vector3f eyeUp;

    Eigen::Matrix4f m_eproj;
    Eigen::Matrix4f m_ecamera;
    Eigen::Matrix4f m_eworld;

    bool m_transparent;

    GLuint m_uniformBufferVs;
    UniformVs m_uniformVs;

    int m_frame = 0;


//----------------------------------- bullet -----------------------------


        btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
        btBroadphaseInterface*	m_broadphase;
        btCollisionDispatcher*	m_dispatcher;
        btConstraintSolver*	m_solver;
        btDefaultCollisionConfiguration* m_collisionConfiguration;




private:


    std::shared_ptr<MouseRenderer> mouse;
    std::list<actorPtr> actors;
    std::list<lightPtr> lights;
    std::list<fbxrendererPtr> fbxs;
    std::list<qmlrendererPtr> qmls;

    qmlrendererPtr currentQml;


};

#endif // SCENE_H
