
#include "renderwindow.h"
#include "cuberenderer.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOffscreenSurface>
#include <QScreen>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QCoreApplication>

#include <unistd.h>

#include "external/Leap.h"
#include "external/cpplinq.hpp"
#include "eigenutil.h"


#include "Kernel/OVR_Threads.h"
#include "Util/Util_SystemGUI.h"


#include "external/external_opengl/glm/glm.hpp"
#include "external/external_opengl/glm/gtc/type_ptr.hpp"


class RenderControl : public QQuickRenderControl
{
public:
    RenderControl(QWindow *w) : m_window(w) { }
    QWindow *renderWindow(QPoint *offset) Q_DECL_OVERRIDE;

private:
    QWindow *m_window;
};

QWindow *RenderControl::renderWindow(QPoint *offset)
{
    if (offset)
        *offset = QPoint(0, 0);
    return m_window;
}

RenderWindow::RenderWindow()
    : m_rootItem(0),
      m_fbo(0),
      m_quickInitialized(false),
      m_quickReady(false),
      m_dpr(0)
{
    qDebug() << "come here render window";
//*****************************init RenderWindow************************//

    position = ThePlayer.getPosition();
    eye = position;
    center= QRRUtil::EigenVector3fMake(0.0f, 0.0f, 0.0f);
    eyeUp= QRRUtil::EigenVector3fMake(0.0f, 1.0f , 0.0f);

    m_ecamera = QRRUtil::lookAt(eye, center, eyeUp);
    m_eworld = Eigen::Matrix4f::Identity();

//*******************************************************************

    qDebug() << "before setsurfacetype";

    setSurfaceType(QSurface::OpenGLSurface);

    QSurfaceFormat format;
    // Qt Quick may need a depth and stencil buffer. Always make sure these are available.
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);
    setFormat(format);

    m_context = new QOpenGLContext;
    m_context->setFormat(format);
    m_context->create();

    m_offscreenSurface = new QOffscreenSurface;

    m_offscreenSurface->setFormat(m_context->format());
    m_offscreenSurface->create();

    m_cubeRenderer = new CubeRenderer(m_offscreenSurface);

    m_renderControl = new RenderControl(this);


    m_quickWindow = new QQuickWindow(m_renderControl);


    m_qmlEngine = new QQmlEngine;
    if (!m_qmlEngine->incubationController())
        m_qmlEngine->setIncubationController(m_quickWindow->incubationController());


    m_updateTimer.setSingleShot(true);
    m_updateTimer.setInterval(16);
    connect(&m_updateTimer, &QTimer::timeout, this, &RenderWindow::render);

    connect(m_quickWindow, &QQuickWindow::sceneGraphInitialized, this, &RenderWindow::createFbo);
    connect(m_quickWindow, &QQuickWindow::sceneGraphInvalidated, this, &RenderWindow::destroyFbo);
    connect(m_renderControl, &QQuickRenderControl::renderRequested, this, &RenderWindow::requestUpdate);
    connect(m_renderControl, &QQuickRenderControl::sceneChanged, this, &RenderWindow::requestUpdate);


    connect(this, &QWindow::screenChanged, this, &RenderWindow::handleScreenChange);

}

RenderWindow::~RenderWindow()
{

    m_context->makeCurrent(m_offscreenSurface);


    delete m_renderControl;

    delete m_qmlComponent;
    delete m_quickWindow;
    delete m_qmlEngine;
    delete m_fbo;

    m_context->doneCurrent();

    delete m_cubeRenderer;

    delete m_offscreenSurface;
    delete m_context;
}

void RenderWindow::createFbo()
{

    m_dpr = devicePixelRatio();
    m_fbo = new QOpenGLFramebufferObject(size() * m_dpr, QOpenGLFramebufferObject::CombinedDepthStencil);
    m_quickWindow->setRenderTarget(m_fbo);
}

void RenderWindow::destroyFbo()
{
    delete m_fbo;
    m_fbo = 0;
}

void RenderWindow::render()
{

    qDebug() << "before render";

    if (!m_context->makeCurrent(m_offscreenSurface))
        return;

    m_renderControl->polishItems();
    m_renderControl->sync();
    m_renderControl->render();

    m_quickWindow->resetOpenGLState();
    QOpenGLFramebufferObject::bindDefault();

    QOpenGLFunctions_3_3_Core* f = 0;
    f = m_context->versionFunctions<QOpenGLFunctions_3_3_Core>();
    f->glFlush();

    m_quickReady = true;

    qDebug() << "before paintrender";


    this->paintrender();

    m_cubeRenderer->render(this, m_context, m_quickReady ? m_fbo->texture() : 0);
}

void RenderWindow::requestUpdate()
{
    if (!m_updateTimer.isActive())
        m_updateTimer.start();
}

void RenderWindow::run()
{
    disconnect(m_qmlComponent, SIGNAL(statusChanged(QQmlComponent::Status)), this, SLOT(run()));

    if (m_qmlComponent->isError()) {
        QList<QQmlError> errorList = m_qmlComponent->errors();
        foreach (const QQmlError &error, errorList)
            qWarning() << error.url() << error.line() << error;
        return;
    }

    QObject *rootObject = m_qmlComponent->create();
    if (m_qmlComponent->isError()) {
        QList<QQmlError> errorList = m_qmlComponent->errors();
        foreach (const QQmlError &error, errorList)
            qWarning() << error.url() << error.line() << error;
        return;
    }

    m_rootItem = qobject_cast<QQuickItem *>(rootObject);
    if (!m_rootItem) {
        qWarning("run: Not a QQuickItem");
        delete rootObject;
        return;
    }


    m_rootItem->setParentItem(m_quickWindow->contentItem());


    updateSizes();

    m_context->makeCurrent(m_offscreenSurface);
    m_renderControl->initialize(m_context);
    m_quickInitialized = true;


    /////////////////////////////
    this->init();

}

void RenderWindow::updateSizes()
{

    m_rootItem->setWidth(width());
    m_rootItem->setHeight(height());

    m_quickWindow->setGeometry(0, 0, width(), height());

    m_cubeRenderer->resize(width(), height());
}

void RenderWindow::startQuick(const QString &filename)
{
    m_qmlComponent = new QQmlComponent(m_qmlEngine, QUrl(filename));
    if (m_qmlComponent->isLoading())
        connect(m_qmlComponent, &QQmlComponent::statusChanged, this, &RenderWindow::run);
    else
        run();
}

unsigned char* RenderWindow::make_dummy_texture (int* width_, int* height_)
{
    int height = 256;
    int width  = 256;
    unsigned char* pixels = (unsigned char*)malloc (width*height*4);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixels[y*(width*4) + x*4 +0] = x % 256;
            pixels[y*(width*4) + x*4 +1] = y % 256;
            pixels[y*(width*4) + x*4 +2] = 0;
            pixels[y*(width*4) + x*4 +3] = 255;
        }
    }
    *width_  = width;
    *height_ = height;
    return pixels;
}



void RenderWindow::init ()
{

    using boost::asio::ip::udp;
    QOpenGLFunctions_3_3_Core* f = 0;
    f = m_context->versionFunctions<QOpenGLFunctions_3_3_Core>();

    using boost::asio::ip::udp;
    QOpenGLFunctions_3_3_Compatibility* fc = 0;
    fc = m_context->versionFunctions<QOpenGLFunctions_3_3_Compatibility>();


    qDebug() << " come this init";

    OnStartup();

    hand_program = new QOpenGLShaderProgram();
    distort_program = new QOpenGLShaderProgram();

    hand_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString("./resources/Shaders/SkinCloth.vert"));
    hand_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString("./resources/Shaders/Skin.frag"));
    hand_program->link();

    distort_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString("./resources/Shaders/rift.vert"));
    distort_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString("./resources/Shaders/rift.frag"));
    distort_program->link();

    qDebug() << "shaderlinking and prepare fbx";


   // hand_program->bind();
    //glUseProgram(hand_program->programId());

    qDebug() << "afterlink" << hand_program->programId();


    qDebug() << "a";
    glUseProgram(hand_program->programId());
    qDebug() << "a";

  //  qDebug() << glGetUniformBlockIndex(shader_program, "vertex");
    qDebug() << f->glGetUniformBlockIndex(hand_program->programId(), "UniformVs");




    uniformsSkin[UNIFORM_VS] = f->glGetUniformBlockIndex(hand_program->programId(), "UniformVs");
    uniformsSkin[UNIFORM_DIFFUSE_TEXTURE] = f->glGetUniformBlockIndex(hand_program->programId(), "diffuseTexture");
    uniformsSkin[UNIFORM_FALLOFF_TEXTURE] = f->glGetUniformBlockIndex(hand_program->programId(), "falloffTexture");
    uniformsSkin[UNIFORM_RIMLIGHT_TEXTURE] = f->glGetUniformBlockIndex(hand_program->programId(), "rimlightTexture");
    uniformsSkin[UNIFORM_SPECULAR_TEXTURE] = f->glGetUniformBlockIndex(hand_program->programId(), "specularTexture");

    qDebug() << "before gen buffefe";

    f->glGenBuffers(1, &m_uniformBufferVs);

    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_TEXTURE_2D);
    f->glEnable(GL_CULL_FACE);


    m_fbxLoader.Initialize("./resources/model/hand_rig_fixed.fbx");
    qDebug() << "initialize ?" << m_fbxLoader.GetMaterialCount() ;
    m_fbxLoader.LoadAnimation("./resources/model/hand_rig_fixed.fbx");
    qDebug() << "load animation ? ";


    /*
    for(int i = 0; i < m_fbxLoader.GetMaterialCount(); ++i)
    {
        auto& modelMaterial = m_fbxLoader.GetMaterial(i);

        AppMaterial material;
        material.modelMaterial = &modelMaterial;

        std::string home = "/Textures/";
        // read texture
        material.diffuseTexture = loadTexture(home + modelMaterial.diffuseTextureName);
        material.falloffTexture = loadTexture(home + modelMaterial.falloffTextureName);
        material.specularTexture = loadTexture(home + modelMaterial.specularTextureName);
        material.rimlightTexture = 0;

            // repatsampler
            f->glGenSamplers(1, &material.repeatSampler);
            f->glSamplerParameteri(material.repeatSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
            f->glSamplerParameteri(material.repeatSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
            f->glSamplerParameteri(material.repeatSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            f->glSamplerParameteri(material.repeatSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // clump sampler
            f->glGenSamplers(1, &material.clampSampler);
            f->glSamplerParameteri(material.clampSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            f->glSamplerParameteri(material.clampSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            f->glSamplerParameteri(material.clampSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            f->glSamplerParameteri(material.clampSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            m_materialList.push_back(material);

    }
*/
    for (int i = 0; i < m_fbxLoader.GetMeshCount(); ++i)
    {
        auto& modelMesh = m_fbxLoader.GetMesh(i);
        auto& modelVertexList = modelMesh.vertexList;
        auto& modelIndexList = modelMesh.indexList;

        AppMesh mesh;
        mesh.modelMeshId = i;
        mesh.modelMesh = &modelMesh;
        mesh.material = 0;//&m_materialList[m_fbxLoader.GetMaterialId(modelMesh.materialName)];

        f->glGenVertexArrays(1, &mesh.vertexArray);
        f->glBindVertexArray(mesh.vertexArray);

        f->glGenBuffers(1, &mesh.vertexBuffer);
        f->glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(modelVertexList[0]) * modelVertexList.size(), modelVertexList.data(), GL_STATIC_DRAW);

        f->glEnableVertexAttribArray(ATTRIB_VERTEX);
        f->glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, position)));
        f->glEnableVertexAttribArray(ATTRIB_NORMAL);
        f->glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, normal)));
        f->glEnableVertexAttribArray(ATTRIB_TEXCOORD0);
        f->glVertexAttribPointer(ATTRIB_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, uv0)));
        f->glEnableVertexAttribArray(ATTRIB_BONE_INDEX);
        f->glVertexAttribIPointer(ATTRIB_BONE_INDEX, 4, GL_UNSIGNED_BYTE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, boneIndex)));
        f->glEnableVertexAttribArray(ATTRIB_BONE_WEIGHT);
        f->glVertexAttribPointer(ATTRIB_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, boneWeight)));

        f->glGenBuffers(1, &mesh.indexBuffer);
        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
        f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(modelIndexList[0]) * modelIndexList.size(), modelIndexList.data(), GL_STATIC_DRAW);

        f->glBindVertexArray(0);


        m_meshlist.push_back(mesh);
    }

    initOVR();
    initFB();

    qDebug() << "finish init ovr init fb";

    m_frame = 0;

    {
        f->glGenVertexArrays (1, &m_quadvertex_arrays);
        f->glBindVertexArray (m_quadvertex_arrays);

        // send vertex array data
        f->glGenBuffers (1, &m_quadpos_array);
        f->glBindBuffer (GL_ARRAY_BUFFER, m_quadpos_array);
        f->glBufferData (GL_ARRAY_BUFFER, sizeof(m_quadpositions), m_quadpositions, GL_STATIC_DRAW);
        f->glVertexAttribPointer     (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        f->glEnableVertexAttribArray (0);

        f->glGenBuffers (1, &m_quadtex_coord_array);
        f->glBindBuffer (GL_ARRAY_BUFFER, m_quadtex_coord_array);
        f->glBufferData (GL_ARRAY_BUFFER, sizeof(m_quadtex_coords), m_quadtex_coords, GL_STATIC_DRAW);
        f->glVertexAttribPointer     (1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        f->glEnableVertexAttribArray (1);

        // and indices
        f->glGenBuffers (1, &m_quadindex_array);
        f->glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);
        f->glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(m_quadindices), m_quadindices, GL_STATIC_DRAW);


        f->glGenSamplers       (1, &m_distortsampler);
        f->glSamplerParameteri (m_distortsampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
        f->glSamplerParameteri (m_distortsampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
        f->glSamplerParameteri (m_distortsampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        f->glSamplerParameteri (m_distortsampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int   width=1000, height=1000, comp;
           unsigned char* pixels = make_dummy_texture (&width, &height);
           std::cout << "texture width  = " << width  << "\n";
           std::cout << "texture height = " << height << "\n";
           assert (pixels != 0);

           f->glGenTextures (1, &m_quadtexture);
           f->glBindTexture (GL_TEXTURE_2D, m_quadtexture);
           f->glTexImage2D  (GL_TEXTURE_2D, 0,
                          GL_RGBA, width, height, 0,
                          GL_RGBA, GL_UNSIGNED_BYTE, pixels);

           // setup sampler
           f->glGenSamplers       (1, &m_quadsampler);
           f->glSamplerParameteri (m_quadsampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
           f->glSamplerParameteri (m_quadsampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
           f->glSamplerParameteri (m_quadsampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
           f->glSamplerParameteri (m_quadsampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


       // f->glBindVertexArray(0);

    }

    qDebug() << size();

//---------------------------------------------net wrokd videl -----------


    /*  sock = socket(AF_INET, SOCK_DGRAM, 0);
       addr.sin_family = AF_INET;
       addr.sin_port = htons(1080);
       addr.sin_addr.s_addr = INADDR_ANY;
       bind(sock, (struct sockaddr *)&addr, sizeof(addr));
       */

    genVideoTexture();


    qDebug() << "finish this init";

}



void RenderWindow::initOVR(){

}

void RenderWindow::initFB(){
    m_rightEyeTex = new QOpenGLFramebufferObject(960,1080, QOpenGLFramebufferObject::CombinedDepthStencil);
    m_leftEyeTex = new QOpenGLFramebufferObject(960,1080, QOpenGLFramebufferObject::CombinedDepthStencil);
    composeEyeTex = new QOpenGLFramebufferObject(960,1080, QOpenGLFramebufferObject::CombinedDepthStencil);
 //   lastcomposeEyeTex = new QOpenGLglFramebufferObject(1000,1000, QOpenGLFramebufferObject::CombinedDepthStencil);
}

void RenderWindow::updateuniform(int index){

    ThePlayer.update();

    qDebug() <<index << "aaaaaaaaaaaaaaaaaaaaaaaaaaa";

    m_uniformVs.lightDirection = EigenVector4fMake(0.0f, 0.0f, 500.0f, 0.0f);
    m_uniformVs.lightDirection = m_uniformVs.lightDirection.normalized();


    m_ecamera = QRRUtil::lookAt(eye, center, eyeUp);
    std::cout << m_ecamera << std::endl;
    if(index==0){
        m_ecamera = ThePlayer.getLeftEyeMat();
    }
    if(index==1){
        m_ecamera = ThePlayer.getRightEyeMat();
    }

    m_uniformVs.normalMatrix = m_eworld.inverse();
    m_uniformVs.modelViewMatrix = m_ecamera * m_eworld;
    m_uniformVs.projectionMatrix = m_eproj;


    //m_rotation += self.timeSinceLastUpdate * 0.5f;

    m_frame += 1.0f;
    if (m_frame >= m_fbxLoader.GetAnimationEndFrame())
    {
        m_frame -= m_fbxLoader.GetAnimationEndFrame();
    }

}

void RenderWindow::paintrender()
{
    QOpenGLFunctions_3_3_Core* f = 0;
    f = m_context->versionFunctions<QOpenGLFunctions_3_3_Core>();

    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_TEXTURE_2D);
    f->glEnable(GL_TEXTURE);
    f->glEnable(GL_CULL_FACE);

    auto drawFunc = [=](const std::vector<AppMesh>& meshlist)
    {
        for (int i = 0; i< meshlist.size(); ++i)
        {
            auto& mesh = meshlist[i];

            GLint* uniforms = nullptr;
            uniforms = uniformsSkin;

            hand_program->bind();

            m_handinfo.UpdateInfo();

            m_fbxLoader.GetMeshMatrix(m_frame, mesh.modelMeshId, m_uniformVs.meshMatrix);
            m_fbxLoader.GetBoneMatrix(m_frame, mesh.modelMeshId, m_uniformVs.boneMatrixList, MAX_BONE_COUNT, &m_handinfo);

            // uniform buffer copy
            f->glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferVs);
            f->glBufferData(GL_UNIFORM_BUFFER, sizeof(m_uniformVs), &m_uniformVs, GL_DYNAMIC_DRAW);
            f->glBindBufferBase(GL_UNIFORM_BUFFER, uniforms[UNIFORM_VS], m_uniformBufferVs);

            f->glBindVertexArray(mesh.vertexArray);
            /*
                f->glActiveTexture(GL_TEXTURE0);
                f->glBindTexture(GL_TEXTURE_2D, mesh.material->diffuseTexture);
                f->glBindSampler(0, mesh.material->repeatSampler);
                f->glUniform1i(uniforms[UNIFORM_DIFFUSE_TEXTURE], 0);

                f->glActiveTexture(GL_TEXTURE1);
                f->glBindTexture(GL_TEXTURE_2D, mesh.material->falloffTexture);
                f->glBindSampler(1, mesh.material->clampSampler);
                f->glUniform1i(uniforms[UNIFORM_FALLOFF_TEXTURE], 1);

                f->glActiveTexture(GL_TEXTURE2);
                f->glBindTexture(GL_TEXTURE_2D, mesh.material->rimlightTexture);
                f->glBindSampler(2, mesh.material->clampSampler);
                f->glUniform1i(uniforms[UNIFORM_RIMLIGHT_TEXTURE], 2);
*/
            /*  if (mesh.material->specularTexture != 0)
                {
                  f->glActiveTexture(GL_TEXTURE3);
                  f->glBindTexture(GL_TEXTURE_2D, mesh.material->specularTexture);
                  f->glBindSampler(3, mesh.material->repeatSampler);
                  f->glUniform1i(uniforms[UNIFORM_SPECULAR_TEXTURE], 3);
                }
*/

            f->glDrawElements(GL_TRIANGLES, (GLsizei)mesh.modelMesh->indexList.size(), GL_UNSIGNED_SHORT, nullptr);
        }
    };


//*******************************************
    m_leftEyeTex->bind();

    f->glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateuniform(0);

    f->glViewport(0,0,960,1080);

    f->glUseProgram(hand_program->programId());


    f->glDisable(GL_BLEND);
    drawFunc(m_meshlist);
    f->glFlush();

    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // drawFunc(m_qmlmesh);

   // DistortQuad(composeEyeTex,lastcomposeEyeTex,lastcomposeEyeTex);

    m_leftEyeTex->release();
//**************************************************::

//:*******************
    m_rightEyeTex->bind();

    f->glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateuniform(1);

    f->glViewport(0,0,960,1080);

    f->glUseProgram(hand_program->programId());


    f->glDisable(GL_BLEND);
    drawFunc(m_meshlist);
    f->glFlush();

    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // drawFunc(m_qmlmesh);

   // DistortQuad(composeEyeTex,lastcomposeEyeTex,lastcomposeEyeTex);

    m_rightEyeTex->release();

//********************************************

    m_context->makeCurrent(m_offscreenSurface);
    auto status = f->glCheckFramebufferStatus(GL_FRAMEBUFFER);
     switch(status)
     {
        case GL_FRAMEBUFFER_COMPLETE:
      //  qDebug() <<"frame buffer status good";
        break;
     default:
        //qDebug() << "frame buffer statsu bads";
         break;
     }

    f->glUseProgram(distort_program->programId());

    qDebug() << size().width();
    f->glViewport(0,0, size().width() / 2.0, size().height());
    f->glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
     f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    f->glDisable(GL_CULL_FACE);
    f->glDisable(GL_BLEND);
    f->glDisable(GL_DEPTH_TEST);
    f->glEnable(GL_TEXTURE);


    f->glActiveTexture (GL_TEXTURE0);
    f->glUniform1i     (f->glGetUniformLocation (distort_program->programId(), "sampler"), 0);
    //f->glBindTexture   (GL_TEXTURE_2D, m_quadtexture)

    f->glBindTexture(GL_TEXTURE_2D, m_leftEyeTex->texture());


    //f->glBindTexture(GL_TEXTURE_2D,videotex_right_id);
     //auto numrcv = recvfrom(sock, buff, video_receiveSize, 0,nullptr, nullptr);
    for(int i=0; i<sizeof(buff) ; i++){
     //   video_ibuff.push_back((uchar)buff[i]);
    }

    using boost::asio::ip::udp;

/*
    boost::asio::io_service io_service;
    udp::socket socket(io_service, udp::endpoint(udp::v4(), 1081));

    boost::array<char,65500> recv_buf;
    udp::endpoint remote_endpoint;
    boost::system::error_code error;
    size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);

    for(int i=0; i<recv_buf.size() ; i++){
        video_ibuff.push_back((uchar)recv_buf.data()[i]);
    }

    video_right_image = cv::imdecode(cv::Mat(video_ibuff), CV_LOAD_IMAGE_COLOR);


    video_ibuff.clear();

    f->glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                  640,480, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, video_right_image.data);
*/
    int width, height;

    f->glBindSampler   (0, m_quadsampler);

    // vertices
    f->glBindVertexArray (m_quadvertex_arrays);
    f->glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);

    f->glDrawElements    (GL_TRIANGLE_STRIP, sizeof( m_quadindices)/sizeof( m_quadindices[0]), GL_UNSIGNED_INT, 0);









        f->glViewport(size().width() / 2.0 ,0, size().width() / 2.0, size().height());

        f->glActiveTexture (GL_TEXTURE1);
        f->glUniform1i     (f->glGetUniformLocation (distort_program->programId(), "sampler"), 1);


        f->glBindTexture(GL_TEXTURE_2D, m_rightEyeTex->texture());

     //   f->glBindTexture(GL_TEXTURE_2D,videotex_left_id);
        using boost::asio::ip::udp;

/*
        boost::asio::io_service io_service2;
        udp::socket socket2(io_service, udp::endpoint(udp::v4(), 1080));

        boost::array<char,65500> recv_buf2;
        udp::endpoint remote_endpoint2;
        boost::system::error_code error2;
        size_t len2 = socket2.receive_from(boost::asio::buffer(recv_buf2), remote_endpoint2, 0, error2);

        for(int i=0; i<recv_buf2.size() ; i++){
            video_ibuff.push_back((uchar)recv_buf2.data()[i]);
        }

         video_left_image = cv::imdecode(cv::Mat(video_ibuff), CV_LOAD_IMAGE_COLOR);


        video_ibuff.clear();

        f->glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                      640,480, 0, GL_RGB,
                      GL_UNSIGNED_BYTE, video_left_image.data);
*/
        f->glBindSampler   (1, m_quadsampler);

        // vertices
        f->glBindVertexArray (m_quadvertex_arrays);
        f->glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);

        f->glDrawElements    (GL_TRIANGLE_STRIP, sizeof( m_quadindices)/sizeof( m_quadindices[0]), GL_UNSIGNED_INT, 0);





    getOculusAngle();


 //   lastcomposeEyeTex = composeEyeTex;
}

static void OVR_CDECL LogCallback(int level, const char* message)
{
    OVR_UNUSED2(level, message);
}

int RenderWindow::OnStartup()
{

    //OVR::Thread::SetCurrentThreadName("OWDMain");


    ovrInitParams params = {0, 0, nullptr, 0};
    params.LogCallback = LogCallback;
    ovrBool result = ovr_Initialize(&params);

    ovr_TraceMessage(ovrLogLevel_Info, "Oculus World Demo OnStartup");

    if (!result)
    {
        qDebug() << "oculusworlddemo", "unabel to initialize libovr";

    }

    Hmd = ovrHmd_Create(0);
    if (!Hmd || !ovrHmd_ConfigureTracking(Hmd,
        ovrTrackingCap_Orientation, 0))
    {
        qDebug() << "Unable to create hmd" << ovrHmd_GetLastError(NULL);
    }

    for (int i = 0; i < 10; ++i) {
      ovrTrackingState state = ovrHmd_GetTrackingState(Hmd, 0);

      ovrQuatf orientation = state.HeadPose.ThePose.Orientation;
      qDebug () << orientation.x << orientation.y << orientation.z << orientation.w;
      glm::quat q = glm::make_quat(&orientation.x);
      glm::vec3 euler = glm::eulerAngles(q);


      qDebug() << euler.x << euler.y << euler.z;

    }


    return 0;

}

void RenderWindow::CalculateHmdValues()
{

}

void RenderWindow::getOculusAngle(){
    ovrTrackingState state = ovrHmd_GetTrackingState(Hmd, 0);

    ovrQuatf orientation = state.HeadPose.ThePose.Orientation;
    qDebug () << orientation.x << orientation.y << orientation.z << orientation.w;
    Eigen::Matrix4f testmat = QRRUtil::MakeMatrixfromQuat( orientation.x , orientation.y , orientation.z , orientation.w);
    std::cout << testmat << std::endl;
    glm::quat q = glm::make_quat(&orientation.x);
    glm::vec3 euler = glm::eulerAngles(q);

 //   qDebug() << euler.x * 180/3.1415<< euler.y * 180/3.1415<< euler.z* 180/3.1415;

    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_1);
    out << euler.x * 180/3.1415<< euler.y * 180/3.1415<< euler.z* 180/3.1415;
    QString send = QString("%1/%2/%3").arg(euler.x * 180/3.1415).arg(euler.y * 180/3.1415).arg(euler.z* 180/3.1415);

    qDebug () << send;
    udpsocket.writeDatagram(send.toLocal8Bit(),send.toLocal8Bit().length(), QHostAddress(QString("192.168.23.201")), 1500);
     udpsocket.writeDatagram(send.toLocal8Bit(),send.toLocal8Bit().length(), QHostAddress::LocalHost, 1500);
}

void RenderWindow::genVideoTexture()
{

    QOpenGLFunctions_3_3_Core* f = 0;
    f = m_context->versionFunctions<QOpenGLFunctions_3_3_Core>();

    f->glGenTextures (1, &videotex_right_id);
    f->glBindTexture (GL_TEXTURE_2D, videotex_right_id);

    video_right_image = cv::Mat(480,640,CV_8UC3);

    f->glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                  640,480, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, video_right_image.data);

    f->glGenTextures (1, &videotex_left_id);
    f->glBindTexture (GL_TEXTURE_2D, videotex_left_id);

    video_left_image = cv::Mat(480,640,CV_8UC3);

    f->glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                  640,480, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, video_left_image.data);
}

GLuint RenderWindow::loadTexture (const std::string & filename)
{
    QOpenGLFunctions_3_3_Core* f = 0;
    f = m_context->versionFunctions<QOpenGLFunctions_3_3_Core>();

    FIBITMAP *dib1 = nullptr;

    if (filename.size() == 0)
    {
        return 0;
    }

    auto it = m_textureDictionary.find(filename);
    if ( it != m_textureDictionary.end())
    {
        return it->second;
    }

    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename.data());

    dib1 = FreeImage_Load(fif, filename.data(), JPEG_DEFAULT);
    if (!dib1)
    {
        std::cerr << "Erreur ouverture d\'image" << std::endl;
        return 0;
    }

    GLuint tex_id = 0;
    int x, y;
    int height, width;


    RGBQUAD rgbquad;


    FREE_IMAGE_TYPE type;
    BITMAPINFOHEADER *header;

    type = FreeImage_GetImageType(dib1);


    height = FreeImage_GetHeight(dib1);
    width = FreeImage_GetWidth(dib1);

    header = FreeImage_GetInfoHeader(dib1);
    int scanLineWidh = ((3*width)%4 == 0) ? 3*width : ((3*width)/4)*4+4;
    unsigned char * texels= (GLubyte*)calloc(height*scanLineWidh, sizeof(GLubyte));
    for (x=0 ; x<width ; x++)
        for (y=0 ; y<height; y++)
        {
            FreeImage_GetPixelColor(dib1,x,y,&rgbquad);

            texels[(y*scanLineWidh+3*x)]=((GLubyte*)&rgbquad)[2];
            texels[(y*scanLineWidh+3*x)+1]=((GLubyte*)&rgbquad)[1];
            texels[(y*scanLineWidh+3*x)+2]=((GLubyte*)&rgbquad)[0];
        }

    f->glGenTextures (1, &tex_id);
    f->glBindTexture (GL_TEXTURE_2D, tex_id);


    f->glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                  width, height, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, texels);
    free(texels);

    m_textureDictionary.insert({filename, tex_id});

    return tex_id;
}

void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed()) {
        m_cubeRenderer->render(this, m_context, m_quickReady ? m_fbo->texture() : 0);
        if (!m_quickInitialized)
            startQuick(QStringLiteral("Test.qml"));
    }
}

void RenderWindow::resizeFbo()
{
    if (m_rootItem && m_context->makeCurrent(m_offscreenSurface)) {
        delete m_fbo;
        createFbo();
        m_context->doneCurrent();
        updateSizes();
        render();
    }
}

void RenderWindow::resizeEvent(QResizeEvent *)
{
    m_eproj = ThePlayer.m_RightEyeCam.m_proj;
    if (m_fbo && m_fbo->size() != size() * devicePixelRatio())
        resizeFbo();
}

void RenderWindow::handleScreenChange()
{
    if (m_dpr != devicePixelRatio())
        resizeFbo();
}

void RenderWindow::mousePressEvent(QMouseEvent *e)
{

    QMouseEvent mappedEvent(e->type(), e->localPos(), e->screenPos(), e->button(), e->buttons(), e->modifiers());
    QCoreApplication::sendEvent(m_quickWindow, &mappedEvent);
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *e)
{
    QMouseEvent mappedEvent(e->type(), e->localPos(), e->screenPos(), e->button(), e->buttons(), e->modifiers());
    QCoreApplication::sendEvent(m_quickWindow, &mappedEvent);
}
