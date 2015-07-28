

#include "renderwindow.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOffscreenSurface>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QCoreApplication>
#include <QQmlContext>

#include "urlutil.h"

#include <QQmlProperty>

RenderWindow::RenderWindow(QMutex *mutex ,const QString &filename)
    : m_rootItem(0),
      m_fbo(0),
      m_quickInitialized(false),
      m_quickReady(false),
      m_scenemutex(nullptr)
{
    setSurfaceType(QSurface::OpenGLSurface);

    m_filename = filename;
    m_scenemutex = mutex;

    QSurfaceFormat format;

    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);

    m_context = new QOpenGLContext;
    m_context->setFormat(format);
    m_context->create();

    m_offscreenSurface = new QOffscreenSurface;

    m_offscreenSurface->setFormat(m_context->format());
    m_offscreenSurface->create();


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

    delete m_offscreenSurface;
    delete m_context;
}

void RenderWindow::init()
{


    m_renderControl = new QQuickRenderControl();
    m_quickWindow = new QQuickWindow(m_renderControl);

    connect(m_quickWindow, &QQuickWindow::sceneGraphInitialized, this, &RenderWindow::createFbo);
    connect(m_quickWindow, &QQuickWindow::sceneGraphInvalidated, this, &RenderWindow::destroyFbo);
    connect(m_renderControl, &QQuickRenderControl::renderRequested, this, &RenderWindow::requestUpdate);
    connect(m_renderControl, &QQuickRenderControl::sceneChanged, this, &RenderWindow::requestUpdate);

    m_qmlEngine = new QQmlEngine;
    if (!m_qmlEngine->incubationController())
        m_qmlEngine->setIncubationController(m_quickWindow->incubationController());

    m_updateTimer.setSingleShot(true);
    m_updateTimer.setInterval(4);
    connect(&m_updateTimer, &QTimer::timeout, this, &RenderWindow::updateQuick);
    m_updateTimer.start();

    qDebug() << "qml init finish";


}

void RenderWindow::createFbo()
{


    qDebug() << "before create fbo";

    m_fbo = new QOpenGLFramebufferObject(m_qmlwidth, m_qmlheight, QOpenGLFramebufferObject::CombinedDepthStencil);
    m_quickWindow->setRenderTarget(m_fbo);

    qDebug() << "createfbo";

}

void RenderWindow::destroyFbo()
{
    delete m_fbo;
    m_fbo = 0;
}

void RenderWindow::requestUpdate()
{
    if (!m_updateTimer.isActive())
        m_updateTimer.start();
}

QOpenGLContext* RenderWindow::getContext(){
    return m_context;
}

QOpenGLFramebufferObject * RenderWindow::getQmlFbo(){
    return m_fbo;
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

    m_qmlwidth = QQmlProperty::read(rootObject, "width").toInt();
    m_qmlheight = QQmlProperty::read(rootObject, "height").toInt();

    qDebug() << "qmlwidth height" << m_qmlwidth << m_qmlheight;

    m_rootItem = qobject_cast<QQuickItem *>(rootObject);
    if (!m_rootItem) {
        qWarning("run: Not a QQuickItem");
        delete rootObject;
        return;
    }

    m_rootItem->setParentItem(m_quickWindow->contentItem());

    qDebug() << "run";

    m_context->makeCurrent(m_offscreenSurface);
    m_renderControl->initialize(m_context);



    m_rootItem->setWidth(m_qmlwidth);
    m_rootItem->setHeight(m_qmlheight);
    m_quickWindow->setGeometry(0, 0, m_qmlwidth, m_qmlheight);

    qDebug() << m_qmlwidth << m_qmlheight;

    m_fbo = new QOpenGLFramebufferObject(m_qmlwidth, m_qmlheight, QOpenGLFramebufferObject::CombinedDepthStencil);
    m_quickWindow->setRenderTarget(m_fbo);


    m_context->doneCurrent();
    m_quickInitialized = true;


}


void RenderWindow::startQuick(const QString &filename)
{
    m_qmlComponent = new QQmlComponent(m_qmlEngine, QUrl(filename));
    if (m_qmlComponent->isLoading())
        connect(m_qmlComponent, &QQmlComponent::statusChanged, this, &RenderWindow::run);
    else
        run();
}

void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed() && !m_quickInitialized)    startQuick(m_filename);

}

void RenderWindow::resizeEvent(QResizeEvent *)
{

    if (m_rootItem && m_context->makeCurrent(m_offscreenSurface)) {
        delete m_fbo;
        createFbo();
        m_context->doneCurrent();

        m_rootItem->setWidth(m_qmlwidth);
        m_rootItem->setHeight(m_qmlheight);
        m_quickWindow->setGeometry(0, 0, m_qmlwidth, m_qmlheight);
    }
}

int RenderWindow::getQmlWidth(){
    return m_qmlwidth;
}

int RenderWindow::getQmlHeight(){
    return m_qmlheight;
}

QSize RenderWindow::getQmlSize(){
    return QSize(m_qmlwidth, m_qmlheight);
}

void RenderWindow::updateQuick()
{

    if(!m_quickInitialized){
        createFbo();
        startQuick(m_filename);
    }

    if (!m_context->makeCurrent(m_offscreenSurface))
    {
        qDebug() << "context miss";
        return;
    }


    m_renderControl->polishItems();
    m_renderControl->sync();
    m_renderControl->render();

    m_quickWindow->resetOpenGLState();
    QOpenGLFramebufferObject::bindDefault();

    m_quickReady = true;

    m_scenemutex->lock();
    qmlimage = m_fbo->toImage();
    m_scenemutex->unlock();

}


void RenderWindow::mousePressEvent(QMouseEvent *e)
{

}

void RenderWindow::mouseReleaseEvent(QMouseEvent *e)
{

}
