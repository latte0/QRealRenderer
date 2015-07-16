#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H


#include <QWindow>
#include <QMatrix4x4>
#include <QTimer>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QOpenGLVertexArrayObject>
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

#include "handFBXLoader.h"
#include "handinfo.h"
#include "camera.h"
#include "rendertexture.h"
#include "shaderobject.h"
#include "vertexbufferobject.h"
#include "player.h"


#include "OVR.h"
#include "OVR_Kernel.h"
#include "Kernel/OVR_Types.h"
#include "Kernel/OVR_Allocator.h"
#include "Kernel/OVR_RefCount.h"
#include "Kernel/OVR_Log.h"
#include "Kernel/OVR_System.h"
#include "Kernel/OVR_Nullptr.h"
#include "Kernel/OVR_String.h"
#include "Kernel/OVR_Array.h"
#include "Kernel/OVR_Timer.h"
#include "Kernel/OVR_DebugHelp.h"
#include "Extras/OVR_Math.h"
#include "Render/Render_Device.h"
#include "OVR_CAPI.h"

//http://stackoverflow.com/questions/17420739/opencv-2-4-5-and-qt5-error-s

#include "LinearMath/btAlignedObjectArray.h"

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;





#define MAX_BONE_COUNT 128

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLFramebufferObject)
QT_FORWARD_DECLARE_CLASS(QOffscreenSurface)
QT_FORWARD_DECLARE_CLASS(QQuickRenderControl)
QT_FORWARD_DECLARE_CLASS(QQuickWindow)
QT_FORWARD_DECLARE_CLASS(QQmlEngine)
QT_FORWARD_DECLARE_CLASS(QQmlComponent)
QT_FORWARD_DECLARE_CLASS(QQuickItem)

class CubeRenderer;

class RenderWindow : public QWindow
{
    Q_OBJECT


public:
    RenderWindow();
    ~RenderWindow();

//----------------------------------scene render--------------------------------

    GLuint loadTexture (const std::string &filename);

    void init();
    void initOVR();
    void initFB();

  //  HmdDesc hmd;
    bool isHmdInitialized = false;
    ovrHmd ovrhmd;


    unsigned int m_distortsampler;


    QOpenGLFramebufferObject* m_rightEyeTex;
    QOpenGLFramebufferObject* m_leftEyeTex;
    QOpenGLFramebufferObject* composeEyeTex;
    QOpenGLFramebufferObject* lastcomposeEyeTex;
    QOpenGLFramebufferObject* finalTexture;



    enum
    {
        UNIFORM_VS,
        UNIFORM_DIFFUSE_TEXTURE,
        UNIFORM_FALLOFF_TEXTURE,
        UNIFORM_RIMLIGHT_TEXTURE,
        UNIFORM_SPECULAR_TEXTURE,
        NUM_UNIFORMS
    };
    GLint uniformsSkin[NUM_UNIFORMS];
    GLint uniformsCloth[NUM_UNIFORMS];

    enum
    {
        ATTRIB_VERTEX,
        ATTRIB_NORMAL,
        ATTRIB_TEXCOORD0,
        ATTRIB_BONE_INDEX,
        ATTRIB_BONE_WEIGHT,
        NUM_ATTRIBUTES
    };

    struct AppMaterial
    {
        GLuint diffuseTexture;
        GLuint falloffTexture;
        GLuint specularTexture;
        GLuint rimlightTexture;
        GLuint repeatSampler;
        GLuint clampSampler;

        bool diffuseHasAlpha;

        const ModelMaterial* modelMaterial;
    };

    struct AppMesh
    {
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint indexBuffer;

        const AppMaterial* material;
        const ModelMesh* modelMesh;

        int modelMeshId;
    };

    struct UniformVs
    {
        Eigen::Matrix4f modelViewMatrix;
        Eigen::Matrix4f projectionMatrix;
        Eigen::Matrix4f normalMatrix;

        Eigen::Vector4f lightDirection;
        Eigen::Vector4f cameraPosition;

        Eigen::Matrix4f meshMatrix;
        Eigen::Matrix4f boneMatrixList[MAX_BONE_COUNT];
    };


protected:
    void updateuniform(int index) ;
    void paintrender();

private:
    void setupVertexAttribs();

    bool m_core;
    int m_xRot;
    int m_yRot;
    int m_zRot;
    QPoint m_lastPos;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *hand_program;
    QOpenGLShaderProgram *distort_program;

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

    Eigen::Vector3f position;

    Camera *m_maincamra;
    Camera *m_secondcamera;

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

    unsigned char* make_dummy_texture (int* width_, int* height_);

    Player              ThePlayer;

//----------------------------------- bullet -----------------------------
    btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

    btBroadphaseInterface*	m_broadphase;

    btCollisionDispatcher*	m_dispatcher;

    btConstraintSolver*	m_solver;

    btDefaultCollisionConfiguration* m_collisionConfiguration;


//------------------------------------- net work -------------------------
public:
    QUdpSocket udpsocket;

    QUdpSocket *videoudpSocket;
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

    //QByteArray video_datagram;

private slots:
 //   void readvideodata();


public:
//-------------------------------------- oculsu  ---------------------------------------

        ovrHmd              Hmd;

    OVR::Vector3f       BlocksCenter;

    bool                ForceZeroHeadMovement;
    bool                VsyncEnabled;
    bool                MultisampleEnabled;

    bool                IsLowPersistence;
    bool                DynamicPrediction;
    bool                DisplaySleep;
    bool                PositionTrackingEnabled;
    bool				PixelLuminanceOverdrive;
    bool                HqAaDistortion;
    bool                MirrorToWindow;

    bool                ScaleAffectsEyeHeight;
    float               PositionTrackingScale;
    float               DesiredPixelDensity;
    float               FovSideTanMax;
    float               FovSideTanLimit; // Limit value for Fov.
    bool                FadedBorder;

    int                 ScreenNumber;
    int                 FirstScreenInCycle;
    bool                SupportsSrgb;
    bool                SupportsMultisampling;

    double              NotificationTimeout;


    enum TimewarpMode
    {
        Timewarp_Off,
        Timewarp_Orientation,
    };

    TimewarpMode        TimewarpOption;

        ovrEyeRenderDesc    EyeRenderDesc[2];

    bool                TimewarpJitDelayEnabled;
    float               TimewarpRenderIntervalInSeconds;

        unsigned            StartTrackingCaps;

    //RenderDevice*       pRender;
  //  RendererParams      RenderParams;


    bool                LinuxFullscreenOnDevice;




    void getOculusAngle();
    void CalculateHmdValues();
    int OnStartup();

//---------------------------------------------------------------------------------------------------

protected:
    void exposeEvent(QExposeEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

private slots:
    void run();

    void createFbo();
    void destroyFbo();
    void render();
    void requestUpdate();
    void handleScreenChange();

private:
    void startQuick(const QString &filename);
    void updateSizes();
    void resizeFbo();

    QOpenGLContext *m_context;
    QOffscreenSurface *m_offscreenSurface;
    QQuickRenderControl *m_renderControl;
    QQuickWindow *m_quickWindow;
    QQmlEngine *m_qmlEngine;
    QQmlComponent *m_qmlComponent;
    QQuickItem *m_rootItem;
    QOpenGLFramebufferObject *m_fbo;
    bool m_quickInitialized;
    bool m_quickReady;
    QTimer m_updateTimer;
    CubeRenderer *m_cubeRenderer;
    qreal m_dpr;
};


#endif // RENDERWINDOW_H
