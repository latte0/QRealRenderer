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

    auto tmpscene = std::make_shared<Scene>(m_context);
 //   tmpscene->addqml(QmlObject());

    currentscene = tmpscene;

    sceneobjects.push_back(tmpscene);

    m_scenerender = std::make_shared<SceneRender>(m_context);
    m_scenerender->resize(1280, 720);

    m_scenerender->show();
    m_scenerender->init();


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

     //   currentscene->update();
        m_scenerender->render(currentscene);

}
