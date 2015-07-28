#include "scenerender.h"

#include <unistd.h>

#include "external/Leap.h"
#include "external/cpplinq.hpp"
#include "eigenutil.h"
#include "fbxstruct.h"

SceneRender::SceneRender(std::shared_ptr<QOpenGLContext> context)
{
    setSurfaceType(QWindow::OpenGLSurface);
    setCursor(Qt::BlankCursor);

    m_context = context;

    setFormat(m_context->format());

    m_eworld = Eigen::Matrix4f::Identity();


    setMouseGrabEnabled(true);
}

SceneRender::~SceneRender()
{

}


void SceneRender::processing()
{

    if(!isExposed()) return;



    if(!m_initialized){
        init();
        return ;
    }

    paint();
}

void SceneRender::setScene(std::shared_ptr<Scene> scene){
    m_scene = scene;
}

void SceneRender::render(std::shared_ptr<Scene> scene){
    setScene(scene);
    paint();
}

void SceneRender::init()
{

    qDebug() << m_context->isValid();
    qDebug() << "m_context" << m_context->makeCurrent(this);


    m_context->versionFunctions<QOpenGLFunctions_3_3_Core>()->initializeOpenGLFunctions();


    cube = new qmlRenderer();
    cube->init(m_context, "movie.qml");
    cube->setCondition(75 ,QRRUtil::EigenVector3fMake(0,0,-200) ,0,0,true);

    qDebug() << "qml setting";

    kyou = new qmlRenderer();
    kyou->init(m_context, "Paint.qml");
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
    back->init(m_context);
*/

    mouse = new MouseRenderer();
    mouse->init(m_context, "");
    mouse->setAttendant(currentQml);


 //   handfbxrender = new handFbxRenderer();
   // handfbxrender->init(m_context, "");


    auto *f = m_context->versionFunctions<QOpenGLFunctions_3_3_Core>();


    distort_program = new QOpenGLShaderProgram();
    distort_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString("./resources/Shaders/rift.vert"));
    distort_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString("./resources/Shaders/rift.frag"));
    distort_program->link();

    initFB();


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
           unsigned char* pixels = QRR::GLUtil::make_dummy_texture (&width, &height);
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


       // glBindVertexArray(0);

    }

    qDebug() << size();

    m_initialized = true;

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


    m_uniformVs.lightDirection = EigenVector4fMake(0.0f, 0.0f, 500.0f, 0.0f);
    m_uniformVs.lightDirection = m_uniformVs.lightDirection.normalized();

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

void SceneRender::paint()
{


    if (!m_context->makeCurrent(this) || !m_initialized)
        return;

    QOpenGLFunctions_3_3_Core* f = 0;
    f = m_context->versionFunctions<QOpenGLFunctions_3_3_Core>();


    f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glDepthFunc(GL_LEQUAL);

    HandInfo::singleton().UpdateInfo();



    auto sceneRender = [&](QRR::EyeSide eye){

        if(eye == QRR::EyeSide::Left) m_leftEyeTex->bind();
        if(eye == QRR::EyeSide::Right) m_rightEyeTex->bind();

        f->glViewport(0,0,960,1080);

        f->glClearDepth(1.0);
        f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        f->glEnable(GL_DEPTH_TEST);
        f->glEnable(GL_TEXTURE_2D);
        f->glEnable(GL_TEXTURE);
        f->glEnable(GL_CULL_FACE);
        f->glFrontFace(GL_CCW);



        f->glViewport(0,0,960,1080);

        updateuniform(eye);

        if(debugmode == 0){
            cube->render(m_context,m_eproj * m_ecamera, HandInfo::singleton().m_fingerdata[1][3].position + Player::singleton().getPosition() + QRRUtil::EigenVector3fMake(0,0,-400/QRR::Environment::mmDiv),Player::singleton().getPosition() /*- mouse->m_centerpos*/);
            kyou->render(m_context,m_eproj * m_ecamera, HandInfo::singleton().m_fingerdata[1][3].position+ Player::singleton().getPosition() + QRRUtil::EigenVector3fMake(0,0,-400/QRR::Environment::mmDiv),Player::singleton().getPosition() /*- mouse->m_centerpos*/);
            mouse->render(m_context,m_eproj * m_ecamera,Eigen::Vector3f::Identity(), Eigen::Vector3f::Identity());
        }

        //back->render(m_context);
        f->glFrontFace(GL_CCW);

    //   handfbxrender->render(m_context, m_uniformVs);

    /*
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    */

        if(eye == QRR::EyeSide::Left) m_leftEyeTex->release();
        if(eye == QRR::EyeSide::Right) m_rightEyeTex->release();

    };



    auto putScene = [&](QRR::EyeSide eye){

        switch(eye){
            case QRR::EyeSide::Left:

                f->glViewport(0,0, size().width() / 2.0, size().height());
                f->glActiveTexture (GL_TEXTURE0);
                f->glUniform1i     (f->glGetUniformLocation (distort_program->programId(), "sampler"), 0);
                f->glBindTexture(GL_TEXTURE_2D, m_leftEyeTex->texture());

                break;
            case QRR::EyeSide::Right:

                f->glViewport(size().width() / 2.0 ,0, size().width() / 2.0, size().height());
                f->glActiveTexture (GL_TEXTURE1);
                f->glUniform1i     (f->glGetUniformLocation (distort_program->programId(), "sampler"), 1);
                f->glBindTexture(GL_TEXTURE_2D, m_rightEyeTex->texture());
                break;
        }



        f->glBindSampler   (0, m_quadsampler);

        // vertices
        f->glBindVertexArray (m_quadvertex_arrays);
        f->glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, m_quadindex_array);

        glDrawElements    (GL_TRIANGLE_STRIP, sizeof( m_quadindices)/sizeof( m_quadindices[0]), GL_UNSIGNED_INT, 0);


    };


    sceneRender(QRR::EyeSide::Left);
    sceneRender(QRR::EyeSide::Right);

    m_context->makeCurrent(this);


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

      putScene(QRR::EyeSide::Left);
      putScene(QRR::EyeSide::Right);

      m_context->swapBuffers(this);

          m_ovrsender.send();


}


void SceneRender::mousePressEvent(QMouseEvent *e)
{
    Eigen::Vector2f winpos(e->pos().x(), e->pos().y());
    winpos[0] = -0.5 + winpos[0] / size().width();
    winpos[1] = -0.5 + winpos[1] / size().height();
    mouse->setWindowPos(winpos);

    QSize qmlsize = currentQml->getQmlSize();


    QMouseEvent mappedEvent(QEvent::MouseButtonPress,  QPointF(qmlsize.width()*  (winpos.x() + 0.5), qmlsize.height() * ( winpos.y()+0.5) ), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
    QCoreApplication::sendEvent(currentQml->m_rwindow->m_quickWindow, &mappedEvent);

    touched = true;
    if(touched){
        QMouseEvent mappedEvent(QEvent::MouseMove,  QPointF(qmlsize.width()*  (winpos.x() + 0.5), qmlsize.height() * ( winpos.y()+0.5)), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
        QCoreApplication::sendEvent(currentQml->m_rwindow->m_quickWindow, &mappedEvent);
    }

/*
    Eigen::Vector2f pos = cube->calcPos(mouse->m_centerpos);

    QMouseEvent mappedEvent(QEvent::MouseButtonPress,  QPointF(1000* pos.x(), 1000 * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
    QCoreApplication::sendEvent(cube->m_rwindow->m_quickWindow, &mappedEvent);
*/

}

void SceneRender::exposeEvent(QExposeEvent *e){

}

void SceneRender::resizeEvent(QResizeEvent *e)
{
    m_eproj = Player::singleton().m_RightEyeCam.m_proj;
}

void SceneRender::mouseReleaseEvent(QMouseEvent *e)
{

    Eigen::Vector2f winpos(e->pos().x(), e->pos().y());
    winpos[0] = -0.5 + winpos[0] / size().width();
    winpos[1] = -0.5 + winpos[1] / size().height();
    mouse->setWindowPos(winpos);

    QSize qmlsize = currentQml->getQmlSize();

        QMouseEvent mappedEvent(QEvent::MouseButtonRelease,  QPointF(qmlsize.width() * (winpos.x() + 0.5), qmlsize.height() *( winpos.y()+0.5)), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
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

        QSize qmlsize = currentQml->getQmlSize();

    if(touched){
        QMouseEvent mappedEvent(QEvent::MouseMove,  QPointF(qmlsize.width() * (winpos.x() + 0.5), qmlsize.height() * ( winpos.y()+0.5) ), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
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
    else if(keyconf(Qt::Key_S)) Player::singleton().toDown();
    else if(keyconf(Qt::Key_A)) Player::singleton().toRight();
    else if(keyconf(Qt::Key_D)) Player::singleton().toLeft();
    else if(keyconf(Qt::Key_R)) Player::singleton().toOver();
    else if(keyconf(Qt::Key_F)) Player::singleton().toBelow();

    else if(keyconf(Qt::Key_B)) debugmode = ++debugmode % 2;


}
