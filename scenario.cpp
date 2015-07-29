#include "scenario.h"

Scenario::Scenario()
{
    QSurfaceFormat format;

    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);

    std::shared_ptr<QOpenGLContext> context(new QOpenGLContext);

    context->setFormat(format);
    context->create();

    m_context = context;


    m_scenerender = std::make_shared<SceneRender>(m_context);
    m_scenerender->resize(1280, 720);

    m_scenerender->show();
    m_scenerender->init();


    auto tmpscene = std::make_shared<Scene>(m_context);



   // tmpscene->addQml( QmlObject{75 ,Eigen::Vector3f{0,0,-200} ,0,0,true, "Paint.qml"} );
    //tmpscene->addQml( QmlObject{20 ,Eigen::Vector3f{20,20,-8} ,-20,-10,true , "Paint.qml"} );

    currentscene = tmpscene;
    m_sceneobjects.push_back(tmpscene);




    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start( 5 );
}


Scenario::~Scenario()
{

}

void Scenario::changeScene(const QString &scenename)
{
  //  scenemap.find(scenename);
}

void Scenario::run(){

       // currentscene->update();
        m_scenerender->render(currentscene);

}
