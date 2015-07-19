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

#include "handFBXLoader.h"
#include "handinfo.h"
#include "camera.h"
#include "rendertexture.h"
#include "shaderobject.h"
#include "vertexbufferobject.h"
#include "player.h"
#include "renderwindow.h"
#include "cuberenderer.h"
#include "backgroundrenderer.h"
#include "mouserenderer.h"
#include "ovrcondition.h"
#include "fbxrenderer.h"

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
class CubeRenderer;
class MouseRenderer;


using namespace Eigen;
using namespace QRRUtil;


class SceneRender : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    SceneRender(QWidget *parent = 0); ~SceneRender();

    GLuint loadTexture (const std::string &filename);

    CubeRenderer *cube;
    CubeRenderer *kyou;
    BackGroundRenderer *back;
    FbxRenderer *fbxrender;
    MouseRenderer *mouse;
    bool touched = false;

    OVRCondition m_ovrsender;

    Eigen::Vector3f m_position;




    void initFB();


    unsigned int m_distortsampler;


    QOpenGLFramebufferObject* m_rightEyeTex;
    QOpenGLFramebufferObject* m_leftEyeTex;
    QOpenGLFramebufferObject* composeEyeTex;
    QOpenGLFramebufferObject* lastcomposeEyeTex;
    QOpenGLFramebufferObject* finalTexture;




protected:
    void initializeGL () override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;


    void updateuniform(int index) ;

private:
    bool m_core;
	int m_xRot;
	int m_yRot;
	int m_zRot;
	QPoint m_lastPos;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLShaderProgram *hand_program;
    QOpenGLShaderProgram *distort_program;

    GLint uniformsSkin[NUM_UNIFORMS];
    GLint uniformsCloth[NUM_UNIFORMS];


	GLuint m_projMatrix;

    HandInfo m_handinfo;


    GLuint m_uniformBufferVs;
    UniformVs m_uniformVs;

    FBXLoader m_fbxLoader;
	std::map<std::string, GLuint> m_textureDictionary;
	std::vector<AppMaterial> m_materialList;
	std::vector<AppMesh> m_meshlist;

    Eigen::Vector3f eye;
    Eigen::Vector3f center;
    Eigen::Vector3f eyeUp;

    Eigen::Matrix4f m_eproj;
    Eigen::Matrix4f m_ecamera;
    Eigen::Matrix4f m_eworld;

	bool m_transparent;

    int m_frame = 0;

    static constexpr float m_mouseSpeed = 0.000005f;
    static constexpr float m_speed = 3.0f;
    float m_horizontalAngle = 3.14f;
    float m_verticalAngle = 0.0f;





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


    Player              ThePlayer;




//----------------------------------- bullet -----------------------------


    btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
    btBroadphaseInterface*	m_broadphase;
    btCollisionDispatcher*	m_dispatcher;
    btConstraintSolver*	m_solver;
    btDefaultCollisionConfiguration* m_collisionConfiguration;


//------------------------------------- net work -------------------------
public:

    int sock, numrcv;
    struct sockaddr_in addr;

    cv::Mat video_right_image;
    cv::Mat video_left_image;
    static constexpr int video_receiveSize = 65500;
    static char buff[video_receiveSize];

    std::vector<uchar> video_ibuff;

    void genVideoTexture();
    GLuint videotex_right_id;
    GLuint videotex_left_id;

//---------------------------------------------------------------------------------------------------


private slots:
    void processing();

};

#endif // GAMEAPPLICATION_H
