#include "scenerender.h"

#include <unistd.h>

#include "external/Leap.h"
#include "external/cpplinq.hpp"
#include "eigenutil.h"
#include "fbxstruct.h"

SceneRender::SceneRender(QWidget *parent)
    : QOpenGLWidget(parent)

{

    m_position = Player::singleton().getPosition();
    m_eworld = Eigen::Matrix4f::Identity();

    setMouseTracking(true);
}

SceneRender::~SceneRender()
{

}


void SceneRender::processing()
{
    update();
}

void SceneRender::initializeGL ()
{
    using boost::asio::ip::udp;


    initializeOpenGLFunctions();

    cube = new qmlRenderer();
    cube->init(this->context(), "movie.qml");
    cube->setCondition(75 ,QRRUtil::EigenVector3fMake(0,0,-200) ,0,0,true);

    kyou = new qmlRenderer();
    kyou->init(this->context(), "imageview.qml");
    kyou->setCondition(20 ,QRRUtil::EigenVector3fMake(20,20,-8) ,-20,-10,true);

    currentQml = kyou;

    qDebug() << "qmlok";

/*
    scenemutex = new QMutex();
    copywindow = new CopyWindow(scenemutex, &sceneimage);
    //copywindow->init();
    copywindow->resize(1920,1080);
    copywindow->show();
*/


/*
    back = new BackGroundRenderer(1091);
    back->init(this->context());
*/

    mouse = new MouseRenderer();
    mouse->init(this->context(), "");
    mouse->setAttendant(currentQml);


 //   handfbxrender = new handFbxRenderer();
   // handfbxrender->init(this->context(), "");





    glClearColor(0, 0, 0, m_transparent ? 0 : 1);


    distort_program = new QOpenGLShaderProgram();
    distort_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString("./resources/Shaders/rift.vert"));
    distort_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString("./resources/Shaders/rift.frag"));
    distort_program->link();

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
           unsigned char* pixels = QRR::GLUtil::make_dummy_texture (&width, &height);
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

}


void SceneRender::initFB(){
    m_rightEyeTex = new QOpenGLFramebufferObject(960,1080, QOpenGLFramebufferObject::Depth);
    m_leftEyeTex = new QOpenGLFramebufferObject(960,1080, QOpenGLFramebufferObject::CombinedDepthStencil);
    composeEyeTex = new QOpenGLFramebufferObject(960,1080, QOpenGLFramebufferObject::CombinedDepthStencil);
    finalTexture = new QOpenGLFramebufferObject(1920,1080, QOpenGLFramebufferObject::CombinedDepthStencil);
 //   lastcomposeEyeTex = new QOpenGLFramebufferObject(1000,1000, QOpenGLFramebufferObject::CombinedDepthStencil);
}

void SceneRender::updateuniform(QRR::EyeSide eye){

    Player::singleton().update();


//    m_uniformVs.lightDirection = EigenVector4fMake(0.0f, 0.0f, 500.0f, 0.0f);
  //  m_uniformVs.lightDirection = m_uniformVs.lightDirection.normalized();

    m_ecamera = Player::singleton().getEyeMat(eye);


    m_eworld =QRRUtil::MakeTransform( Player::singleton().getPosition() + QRRUtil::EigenVector3fMake(0,0,-400/QRR::Environment::mmDiv));


    m_uniformVs.normalMatrix = m_eworld.inverse();
    m_uniformVs.modelViewMatrix = m_ecamera * m_eworld;
    m_uniformVs.projectionMatrix = m_eproj;


    m_frame += 1.0f;

    /*if (m_frame >= m_fbxLoader.GetAnimationEndFrame())
    {
        m_frame -= m_fbxLoader.GetAnimationEndFrame();
    }
    */

}

void SceneRender::paintGL()
{


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LEQUAL);

    HandInfo::singleton().UpdateInfo();



    auto sceneRender = [=](QRR::EyeSide eye){

        if(eye == QRR::EyeSide::Left) m_leftEyeTex->bind();
        if(eye == QRR::EyeSide::Right) m_rightEyeTex->bind();

        glViewport(0,0,960,1080);

        glClearDepth(1.0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);



        glViewport(0,0,960,1080);

        updateuniform(eye);

        if(debugmode == 0){
            cube->render(this->context(),m_eproj * m_ecamera, HandInfo::singleton().m_fingerdata[1][3].position + Player::singleton().getPosition() + QRRUtil::EigenVector3fMake(0,0,-400/QRR::Environment::mmDiv),Player::singleton().getPosition() /*- mouse->m_centerpos*/);
            kyou->render(this->context(),m_eproj * m_ecamera, HandInfo::singleton().m_fingerdata[1][3].position+ Player::singleton().getPosition() + QRRUtil::EigenVector3fMake(0,0,-400/QRR::Environment::mmDiv),Player::singleton().getPosition() /*- mouse->m_centerpos*/);
            mouse->render(this->context(),m_eproj * m_ecamera,Eigen::Vector3f::Identity(), Eigen::Vector3f::Identity());
        }

        //back->render(this->context());
        glFrontFace(GL_CCW);

    //   handfbxrender->render(this->context(),&HandInfo::singleton(), m_uniformVs);

    /*
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    */

        if(eye == QRR::EyeSide::Left) m_leftEyeTex->release();
        if(eye == QRR::EyeSide::Right) m_rightEyeTex->release();

    };



    auto putScene = [=](QRR::EyeSide eye){

        switch(eye){
            case QRR::EyeSide::Left:

                glViewport(0,0, size().width() / 2.0, size().height());
                glActiveTexture (GL_TEXTURE0);
                glUniform1i     (glGetUniformLocation (distort_program->programId(), "sampler"), 0);
                                glBindTexture(GL_TEXTURE_2D, m_leftEyeTex->texture());

                break;
            case QRR::EyeSide::Right:

                glViewport(size().width() / 2.0 ,0, size().width() / 2.0, size().height());
                glActiveTexture (GL_TEXTURE1);
                glUniform1i     (glGetUniformLocation (distort_program->programId(), "sampler"), 1);
                                glBindTexture(GL_TEXTURE_2D, m_rightEyeTex->texture());
                break;
        }



        glBindSampler   (0, m_quadsampler);

        // vertices
        glBindVertexArray (m_quadvertex_arrays);
        glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);

        glDrawElements    (GL_TRIANGLE_STRIP, sizeof( m_quadindices)/sizeof( m_quadindices[0]), GL_UNSIGNED_INT, 0);


    };


    sceneRender(QRR::EyeSide::Left);
    sceneRender(QRR::EyeSide::Right);


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

      putScene(QRR::EyeSide::Left);
      putScene(QRR::EyeSide::Right);

          m_ovrsender.send();


}


void SceneRender::resizeGL(int width, int height)
{

    m_eproj = Player::singleton().m_RightEyeCam.m_proj;
}

void SceneRender::mousePressEvent(QMouseEvent *e)
{
    Eigen::Vector2f winpos(e->pos().x(), e->pos().y());
    winpos[0] = -0.5 + winpos[0] / size().width();
    winpos[1] = -0.5 + winpos[1] / size().height();
    mouse->setWindowPos(winpos);

    QMouseEvent mappedEvent(QEvent::MouseButtonPress,  QPointF(1000* winpos.x(), 1000 * winpos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
    QCoreApplication::sendEvent(kyou->m_rwindow->m_quickWindow, &mappedEvent);

    touched = true;
    if(touched){
        QMouseEvent mappedEvent(QEvent::MouseMove,  QPointF(1000* winpos.x(), 1000 * winpos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
        QCoreApplication::sendEvent(currentQml->m_rwindow->m_quickWindow, &mappedEvent);
    }

/*
    Eigen::Vector2f pos = cube->calcPos(mouse->m_centerpos);

    QMouseEvent mappedEvent(QEvent::MouseButtonPress,  QPointF(1000* pos.x(), 1000 * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
    QCoreApplication::sendEvent(cube->m_rwindow->m_quickWindow, &mappedEvent);
*/

}

void SceneRender::mouseReleaseEvent(QMouseEvent *e)
{

    Eigen::Vector2f winpos(e->pos().x(), e->pos().y());
    winpos[0] = -0.5 + winpos[0] / size().width();
    winpos[1] = -0.5 + winpos[1] / size().height();
    mouse->setWindowPos(winpos);


        QMouseEvent mappedEvent(QEvent::MouseButtonRelease,  QPointF(1000* winpos.x(), 1000 * winpos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
        QCoreApplication::sendEvent(currentQml->m_rwindow->m_quickWindow, &mappedEvent);

/*
    Eigen::Vector2f pos = cube->calcPos(mouse->m_centerpos);

    std::cout << "mousemoveeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"  << pos * 1000 << std::endl;

    QMouseEvent mappedEvent(QEvent::MouseButtonRelease,  QPointF(1000* pos.x(), 1000 * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
    QCoreApplication::sendEvent(cube->m_rwindow->m_quickWindow, &mappedEvent);
*/
    touched  = false;
}

void SceneRender::mouseMoveEvent(QMouseEvent *e)
{

    Eigen::Vector2f winpos(e->pos().x(), e->pos().y());
    winpos[0] = -0.5 + winpos[0] / size().width();
    winpos[1] = -0.5 + winpos[1] / size().height();
    mouse->setWindowPos(winpos);

    if(touched){
        QMouseEvent mappedEvent(QEvent::MouseMove,  QPointF(1000* winpos.x(), 1000 * winpos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
        QCoreApplication::sendEvent(currentQml->m_rwindow->m_quickWindow, &mappedEvent);
    }

    /*
    mouse->m_rightrot -= (e->pos().x() - m_lastPos.x()) /4.0;
    mouse->m_uprot += (e->pos().y() - m_lastPos.y() ) /4.0;

    m_lastPos = e->pos();
*/

  //  mouse->m_rightrot = -(-60 + (e->pos().y()*(891/256) / (11*2.0)  ));

//    mouse->m_uprot = -50 +  (e->pos().x() ) / (11.0 * 1.7);
/*
    if(touched){
        Eigen::Vector2f pos = cube->calcPos(mouse->m_centerpos);

        QMouseEvent mappedEvent(QEvent::MouseMove,  QPointF(1000* pos.x(), 1000 * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
        QCoreApplication::sendEvent(cube->m_rwindow->m_quickWindow, &mappedEvent);
    }
*/
/*
    QPoint glob = mapToGlobal(QPoint(width()/2,height()/2));
    QCursor::setPos(glob);
    m_lastPos = QPoint(width()/2,height()/2);
*/
}


void SceneRender::keyPressEvent(QKeyEvent *e){

    auto keyconf = [e](int k) -> bool
    {
        return (e->key() == k);
    };

    if(keyconf(Qt::Key_W)) Player::singleton().toUp();
    if(keyconf(Qt::Key_S)) Player::singleton().toDown();
    if(keyconf(Qt::Key_A)) Player::singleton().toRight();
    if(keyconf(Qt::Key_D)) Player::singleton().toLeft();
    if(keyconf(Qt::Key_R)) Player::singleton().toOver();
    if(keyconf(Qt::Key_F)) Player::singleton().toBelow();

    if(keyconf(Qt::Key_B)) debugmode = ++debugmode % 2;


}
