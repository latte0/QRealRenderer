#include "scenerender.h"

#include <unistd.h>

#include "external/Leap.h"
#include "external/cpplinq.hpp"
#include "eigenutil.h"


#include "Kernel/OVR_Threads.h"
#include "Util/Util_SystemGUI.h"


#include "external/external_opengl/glm/glm.hpp"
#include "external/external_opengl/glm/gtc/type_ptr.hpp"


SceneRender::SceneRender(QWidget *parent)
    : QOpenGLWidget(parent),
      m_maincamra(nullptr),
      m_secondcamera(nullptr)
{
    position = EigenVector3fMake(50.0f, 30.0f, 20.0f);

    eye = position;
    center= QRRUtil::EigenVector3fMake(0.0f, 0.0f, 0.0f);
    eyeUp= QRRUtil::EigenVector3fMake(0.0f, 1.0f , 0.0f);

    m_ecamera = QRRUtil::lookAt(eye, center, eyeUp);
    m_eworld = Eigen::Matrix4f::Identity();

} 

SceneRender::~SceneRender()
{

}

unsigned char* SceneRender::make_dummy_texture (int* width_, int* height_)
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

void SceneRender::DrawAxisAlignedQuad(float afLowerLeftX, float afLowerLeftY, float afUpperRightX, float afUpperRightY)
{
 //   glDisable(GL_DEPTH_TEST);

    glBindVertexArray (m_quadvertex_arrays);
    glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);

    glUniform1i     (glGetUniformLocation (distort_program->programId(), "sampler"), 0);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture   (GL_TEXTURE_2D, m_quadtexture);
    glBindSampler   (0, m_quadsampler);

    glDrawElements    (GL_TRIANGLE_STRIP, sizeof(m_quadindices)/sizeof(m_quadindices[0]), GL_UNSIGNED_INT, 0);


//    glDisableVertexAttribArray(0);
 //   glDisableVertexAttribArray(1);
}

void SceneRender::DistortQuad(QOpenGLFramebufferObject *src, QOpenGLFramebufferObject *src2, QOpenGLFramebufferObject *dest)
{
       // dest->bind();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glUseProgram(distort_program->programId());
       // glViewport(300, 300, size().width(), size().height());
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    glEnable(GL_TEXTURE);

/*
        glBindVertexArray (m_quadvertex_arrays);
        glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);

        qDebug() << glGetUniformLocation (distort_program->programId(), "sampler");
       /* glUniform1i     (glGetUniformLocation (distort_program->programId(), "sampler"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindSampler   (0, m_distortsampler);
        glBindTexture   (GL_TEXTURE_2D, src->texture());
     /*   glUniform1i     (glGetUniformLocation (distort_program->programId(), "lastFrame"), 1);
        glActiveTexture(GL_TEXTURE1);
        glBindSampler   (1, m_distortsampler);
        glBindTexture   (GL_TEXTURE_2D, src->texture());
*/

    glActiveTexture (GL_TEXTURE0);
     glUniform1i     (glGetUniformLocation (distort_program->programId(), "sampler"), 0);
    glBindTexture   (GL_TEXTURE_2D, m_quadtexture);
    glBindSampler   (0, m_quadsampler);

    // vertices
    glBindVertexArray (m_quadvertex_arrays);
    glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);

    glDrawElements    (GL_TRIANGLE_STRIP, sizeof( m_quadindices)/sizeof( m_quadindices[0]), GL_UNSIGNED_INT, 0);

       // DrawAxisAlignedQuad(-1, -1, 1, 1);
}

void SceneRender::processing()
{
    update();
}

void SceneRender::initializeGL ()
{
    //

    using boost::asio::ip::udp;

    //
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, m_transparent ? 0 : 1);

    OnStartup();

    hand_program = new QOpenGLShaderProgram();
    distort_program = new QOpenGLShaderProgram();

    hand_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString("./resources/Shaders/SkinCloth.vert"));
    hand_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString("./resources/Shaders/Skin.frag"));
    hand_program->link();

   // distort_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString("./resources/Shaders/simpleSample.vert"));
   // distort_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString("./resources/Shaders/simpleSample.frag"));
    distort_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString("./resources/Shaders/rift.vert"));
    distort_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString("./resources/Shaders/rift.frag"));
    distort_program->link();

    glUseProgram(hand_program->programId());

    uniformsSkin[UNIFORM_VS] = glGetUniformBlockIndex(hand_program->programId(), "UniformVs");
    uniformsSkin[UNIFORM_DIFFUSE_TEXTURE] = glGetUniformBlockIndex(hand_program->programId(), "diffuseTexture");
    uniformsSkin[UNIFORM_FALLOFF_TEXTURE] = glGetUniformBlockIndex(hand_program->programId(), "falloffTexture");
    uniformsSkin[UNIFORM_RIMLIGHT_TEXTURE] = glGetUniformBlockIndex(hand_program->programId(), "rimlightTexture");
    uniformsSkin[UNIFORM_SPECULAR_TEXTURE] = glGetUniformBlockIndex(hand_program->programId(), "specularTexture");

    glGenBuffers(1, &m_uniformBufferVs);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);


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
            glGenSamplers(1, &material.repeatSampler);
            glSamplerParameteri(material.repeatSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glSamplerParameteri(material.repeatSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glSamplerParameteri(material.repeatSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glSamplerParameteri(material.repeatSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // clump sampler
            glGenSamplers(1, &material.clampSampler);
            glSamplerParameteri(material.clampSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glSamplerParameteri(material.clampSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glSamplerParameteri(material.clampSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glSamplerParameteri(material.clampSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

        glGenVertexArrays(1, &mesh.vertexArray);
        glBindVertexArray(mesh.vertexArray);

        glGenBuffers(1, &mesh.vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(modelVertexList[0]) * modelVertexList.size(), modelVertexList.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(ATTRIB_VERTEX);
        glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, position)));
        glEnableVertexAttribArray(ATTRIB_NORMAL);
        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, normal)));
        glEnableVertexAttribArray(ATTRIB_TEXCOORD0);
        glVertexAttribPointer(ATTRIB_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, uv0)));
        glEnableVertexAttribArray(ATTRIB_BONE_INDEX);
        glVertexAttribIPointer(ATTRIB_BONE_INDEX, 4, GL_UNSIGNED_BYTE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, boneIndex)));
        glEnableVertexAttribArray(ATTRIB_BONE_WEIGHT);
        glVertexAttribPointer(ATTRIB_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), BUFFER_OFFSET(offsetof(ModelVertex, boneWeight)));

        glGenBuffers(1, &mesh.indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(modelIndexList[0]) * modelIndexList.size(), modelIndexList.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);


        m_meshlist.push_back(mesh);
    }

    initOVR();
    initFB();

    m_frame = 0;

    {
        glGenVertexArrays (1, &m_quadvertex_arrays);
        glBindVertexArray (m_quadvertex_arrays);

        // send vertex array data
        glGenBuffers (1, &m_quadpos_array);
        glBindBuffer (GL_ARRAY_BUFFER, m_quadpos_array);
        glBufferData (GL_ARRAY_BUFFER, sizeof(m_quadpositions), m_quadpositions, GL_STATIC_DRAW);
        glVertexAttribPointer     (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray (0);

        glGenBuffers (1, &m_quadtex_coord_array);
        glBindBuffer (GL_ARRAY_BUFFER, m_quadtex_coord_array);
        glBufferData (GL_ARRAY_BUFFER, sizeof(m_quadtex_coords), m_quadtex_coords, GL_STATIC_DRAW);
        glVertexAttribPointer     (1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray (1);

        // and indices
        glGenBuffers (1, &m_quadindex_array);
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(m_quadindices), m_quadindices, GL_STATIC_DRAW);


        glGenSamplers       (1, &m_distortsampler);
        glSamplerParameteri (m_distortsampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri (m_distortsampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glSamplerParameteri (m_distortsampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri (m_distortsampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int   width=1000, height=1000, comp;
           unsigned char* pixels = make_dummy_texture (&width, &height);
           std::cout << "texture width  = " << width  << "\n";
           std::cout << "texture height = " << height << "\n";
           assert (pixels != 0);

           glGenTextures (1, &m_quadtexture);
           glBindTexture (GL_TEXTURE_2D, m_quadtexture);
           glTexImage2D  (GL_TEXTURE_2D, 0,
                          GL_RGBA, width, height, 0,
                          GL_RGBA, GL_UNSIGNED_BYTE, pixels);

           // setup sampler
           glGenSamplers       (1, &m_quadsampler);
           glSamplerParameteri (m_quadsampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
           glSamplerParameteri (m_quadsampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
           glSamplerParameteri (m_quadsampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
           glSamplerParameteri (m_quadsampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


       // glBindVertexArray(0);

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

}



void SceneRender::initOVR(){

}

void SceneRender::initFB(){
    composeEyeTex = new QOpenGLFramebufferObject(960,1080, QOpenGLFramebufferObject::CombinedDepthStencil);
 //   lastcomposeEyeTex = new QOpenGLFramebufferObject(1000,1000, QOpenGLFramebufferObject::CombinedDepthStencil);
}

void SceneRender::updateuniform(){
    m_uniformVs.lightDirection = EigenVector4fMake(0.0f, 0.0f, 500.0f, 0.0f);
    m_uniformVs.lightDirection = m_uniformVs.lightDirection.normalized();

    m_uniformVs.cameraPosition = EigenVector4fMake(0.0f, 10.0f, 60.0f, 0.0f);

    position = EigenVector3fMake(100.0f, 10.0f, 60.0f);

    Eigen::Quaternionf quat;
    Eigen::Vector3f axis;
    axis<<0,0,1;
    quat=AngleAxisf(M_PI * (m_frame / 100.0),axis);
    eye = position;
    center= QRRUtil::EigenVector3fMake(0.0f, 0.0f, 0.0f);
    eyeUp= QRRUtil::EigenVector3fMake(0.0f, 1.0f , 0.0f);

    // eye =  quat * eye;
    // eyeUp = quat * eyeUp;

    // m_ecamera = QRRUtil::lookAt(eye, center, eyeUp);


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

void SceneRender::paintGL()
{

    composeEyeTex->bind();

    glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateuniform();

    glViewport(0,0,960,1080);

    glUseProgram(hand_program->programId());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE);
    glEnable(GL_CULL_FACE);

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
            glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferVs);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(m_uniformVs), &m_uniformVs, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, uniforms[UNIFORM_VS], m_uniformBufferVs);

            glBindVertexArray(mesh.vertexArray);
            /*
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mesh.material->diffuseTexture);
                glBindSampler(0, mesh.material->repeatSampler);
                glUniform1i(uniforms[UNIFORM_DIFFUSE_TEXTURE], 0);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, mesh.material->falloffTexture);
                glBindSampler(1, mesh.material->clampSampler);
                glUniform1i(uniforms[UNIFORM_FALLOFF_TEXTURE], 1);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, mesh.material->rimlightTexture);
                glBindSampler(2, mesh.material->clampSampler);
                glUniform1i(uniforms[UNIFORM_RIMLIGHT_TEXTURE], 2);
*/
            /*  if (mesh.material->specularTexture != 0)
                {
                  glActiveTexture(GL_TEXTURE3);
                  glBindTexture(GL_TEXTURE_2D, mesh.material->specularTexture);
                  glBindSampler(3, mesh.material->repeatSampler);
                  glUniform1i(uniforms[UNIFORM_SPECULAR_TEXTURE], 3);
                }
*/

            glDrawElements(GL_TRIANGLES, (GLsizei)mesh.modelMesh->indexList.size(), GL_UNSIGNED_SHORT, nullptr);
        }
    };

    glDisable(GL_BLEND);
    drawFunc(m_meshlist);
    glFlush();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // drawFunc(m_qmlmesh);

   // DistortQuad(composeEyeTex,lastcomposeEyeTex,lastcomposeEyeTex);

    composeEyeTex->release();

    makeCurrent();
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
     switch(status)
     {
        case GL_FRAMEBUFFER_COMPLETE:
      //  qDebug() <<"frame buffer status good";
        break;
     default:
        //qDebug() << "frame buffer statsu bads";
         break;
     }

    glUseProgram(distort_program->programId());

    qDebug() << size().width();
    glViewport(0,0, size().width() / 2.0, size().height());
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE);


    glActiveTexture (GL_TEXTURE0);
    glUniform1i     (glGetUniformLocation (distort_program->programId(), "sampler"), 0);
    //glBindTexture   (GL_TEXTURE_2D, m_quadtexture)

    glBindTexture(GL_TEXTURE_2D, composeEyeTex->texture());


    //glBindTexture(GL_TEXTURE_2D,videotex_right_id);
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

    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                  640,480, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, video_right_image.data);
*/
    int width, height;

    glBindSampler   (0, m_quadsampler);

    // vertices
    glBindVertexArray (m_quadvertex_arrays);
    glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);

    glDrawElements    (GL_TRIANGLE_STRIP, sizeof( m_quadindices)/sizeof( m_quadindices[0]), GL_UNSIGNED_INT, 0);









        glViewport(size().width() / 2.0 ,0, size().width() / 2.0, size().height());

        glActiveTexture (GL_TEXTURE1);
        glUniform1i     (glGetUniformLocation (distort_program->programId(), "sampler"), 1);


        glBindTexture(GL_TEXTURE_2D, composeEyeTex->texture());

     //   glBindTexture(GL_TEXTURE_2D,videotex_left_id);
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

        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                      640,480, 0, GL_RGB,
                      GL_UNSIGNED_BYTE, video_left_image.data);
*/
        glBindSampler   (1, m_quadsampler);

        // vertices
        glBindVertexArray (m_quadvertex_arrays);
        glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);

        glDrawElements    (GL_TRIANGLE_STRIP, sizeof( m_quadindices)/sizeof( m_quadindices[0]), GL_UNSIGNED_INT, 0);



    getOculusAngle();


 //   lastcomposeEyeTex = composeEyeTex;
}

static void OVR_CDECL LogCallback(int level, const char* message)
{
    OVR_UNUSED2(level, message);
}

int SceneRender::OnStartup()
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

void SceneRender::CalculateHmdValues()
{

}

void SceneRender::getOculusAngle(){
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

void SceneRender::genVideoTexture()
{
    glGenTextures (1, &videotex_right_id);
    glBindTexture (GL_TEXTURE_2D, videotex_right_id);

    video_right_image = cv::Mat(480,640,CV_8UC3);

    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                  640,480, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, video_right_image.data);

    glGenTextures (1, &videotex_left_id);
    glBindTexture (GL_TEXTURE_2D, videotex_left_id);

    video_left_image = cv::Mat(480,640,CV_8UC3);

    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                  640,480, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, video_left_image.data);
}

GLuint SceneRender::loadTexture (const std::string & filename)
{
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

    glGenTextures (1, &tex_id);
    glBindTexture (GL_TEXTURE_2D, tex_id);


    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
                  width, height, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, texels);
    free(texels);

    m_textureDictionary.insert({filename, tex_id});

    return tex_id;
}


void SceneRender::resizeGL(int width, int height)
{
    m_eproj = QRRUtil::perspective(45.0f, (float)(width /2 )/ height, 0.1f, 10000.0f);
}
void SceneRender::mousePressEvent(QMouseEvent *e)
{
    m_lastPos = e->pos();
}

void SceneRender::mouseMoveEvent(QMouseEvent *e)
{

}
