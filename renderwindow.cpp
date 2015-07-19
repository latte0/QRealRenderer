

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

RenderWindow::RenderWindow(QMutex *mutex)
    : m_rootItem(0),
      m_fbo(0),
      m_program(0),
      m_vbo(0),
      m_quickInitialized(false),
      m_quickReady(false),
      m_scenemutex(nullptr)
{
    setSurfaceType(QSurface::OpenGLSurface);

        m_scenemutex = mutex;

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

    m_renderControl = new QQuickRenderControl(this);


    m_quickWindow = new QQuickWindow(m_renderControl);


    m_qmlEngine = new QQmlEngine;
    if (!m_qmlEngine->incubationController())
        m_qmlEngine->setIncubationController(m_quickWindow->incubationController());

    m_updateTimer.setSingleShot(true);
    m_updateTimer.setInterval(8);
    connect(&m_updateTimer, &QTimer::timeout, this, &RenderWindow::updateQuick);

    connect(m_quickWindow, &QQuickWindow::sceneGraphInitialized, this, &RenderWindow::createFbo);
    connect(m_quickWindow, &QQuickWindow::sceneGraphInvalidated, this, &RenderWindow::destroyFbo);
    connect(m_renderControl, &QQuickRenderControl::renderRequested, this, &RenderWindow::requestUpdate);
    connect(m_renderControl, &QQuickRenderControl::sceneChanged, this, &RenderWindow::requestUpdate);


}

RenderWindow::~RenderWindow()
{

    m_context->makeCurrent(m_offscreenSurface);

    delete m_renderControl;

    delete m_qmlComponent;
    delete m_quickWindow;
    delete m_qmlEngine;
    delete m_fbo;
    delete m_program;
    delete m_vbo;
    delete m_vao;

    m_context->doneCurrent();

    delete m_offscreenSurface;
    delete m_context;
}

void RenderWindow::createFbo()
{

    m_fbo = new QOpenGLFramebufferObject(size() * devicePixelRatio(), QOpenGLFramebufferObject::CombinedDepthStencil);
    m_quickWindow->setRenderTarget(m_fbo);
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

    // The root item is ready. Associate it with the window.
    m_rootItem->setParentItem(m_quickWindow->contentItem());

    // Update item and rendering related geometries.
    updateSizes();

    // Initialize the render control and our OpenGL resources.
    m_context->makeCurrent(m_offscreenSurface);
    m_renderControl->initialize(m_context);

    static const char *vertexShaderSource =
        "attribute highp vec4 vertex;\n"
        "attribute lowp vec2 coord;\n"
        "varying lowp vec2 v_coord;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "   v_coord = coord;\n"
        "   gl_Position = matrix * vertex;\n"
        "}\n";
    static const char *fragmentShaderSource =
        "varying lowp vec2 v_coord;\n"
        "uniform sampler2D sampler;\n"
        "void main() {\n"
        "   vec4 riftcolor = texture2D(sampler,  v_coord);"
        "   gl_FragColor = vec4(riftcolor.b, riftcolor.g, riftcolor.r, 0);\n"
        "}\n";
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("coord", 1);
    m_program->link();
    m_matrixLoc = m_program->uniformLocation("matrix");

    m_vao = new QOpenGLVertexArrayObject;
    m_vao->create();
    m_vao->bind();

    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();

    GLfloat v[] = {
        -0.5, 0.5, 0.5, 0.5,-0.5,0.5,-0.5,-0.5,0.5,
        0.5, -0.5, 0.5, -0.5,0.5,0.5,0.5,0.5,0.5,
        -0.5, -0.5, -0.5, 0.5,-0.5,-0.5,-0.5,0.5,-0.5,
        0.5, 0.5, -0.5, -0.5,0.5,-0.5,0.5,-0.5,-0.5,

        0.5, -0.5, -0.5, 0.5,-0.5,0.5,0.5,0.5,-0.5,
        0.5, 0.5, 0.5, 0.5,0.5,-0.5,0.5,-0.5,0.5,
        -0.5, 0.5, -0.5, -0.5,-0.5,0.5,-0.5,-0.5,-0.5,
        -0.5, -0.5, 0.5, -0.5,0.5,-0.5,-0.5,0.5,0.5,

        0.5, 0.5,  -0.5, -0.5, 0.5,  0.5,  -0.5,  0.5,  -0.5,
        -0.5,  0.5,  0.5,  0.5,  0.5,  -0.5, 0.5, 0.5,  0.5,
        -0.5,  -0.5, -0.5, -0.5, -0.5, 0.5,  0.5, -0.5, -0.5,
        0.5, -0.5, 0.5,  0.5,  -0.5, -0.5, -0.5,  -0.5, 0.5
    };
    GLfloat texCoords[] = {
        0.0f,0.0f, 1.0f,1.0f, 1.0f,0.0f,
        1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
        1.0f,1.0f, 1.0f,0.0f, 0.0f,1.0f,
        0.0f,0.0f, 0.0f,1.0f, 1.0f,0.0f,

        1.0f,1.0f, 1.0f,0.0f, 0.0f,1.0f,
        0.0f,0.0f, 0.0f,1.0f, 1.0f,0.0f,
        0.0f,0.0f, 1.0f,1.0f, 1.0f,0.0f,
        1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,

        0.0f,1.0f, 1.0f,0.0f, 1.0f,1.0f,
        1.0f,0.0f, 0.0f,1.0f, 0.0f,0.0f,
        1.0f,0.0f, 1.0f,1.0f, 0.0f,0.0f,
        0.0f,1.0f, 0.0f,0.0f, 1.0f,1.0f
    };

    const int vertexCount = 36;
    m_vbo->allocate(sizeof(GLfloat) * vertexCount * 5);
    m_vbo->write(0, v, sizeof(GLfloat) * vertexCount * 3);
    m_vbo->write(sizeof(GLfloat) * vertexCount * 3, texCoords, sizeof(GLfloat) * vertexCount * 2);
    m_vbo->release();

    if (m_vao->isCreated())
        setupVertexAttribs();

    // Must unbind before changing the current context. Hence the absence of
    // QOpenGLVertexArrayObject::Binder here.
    m_vao->release();

    m_context->doneCurrent();
    m_quickInitialized = true;
}

void RenderWindow::updateSizes()
{
    // Behave like SizeRootObjectToView.
    m_rootItem->setWidth(width());
    m_rootItem->setHeight(height());

    m_quickWindow->setGeometry(0, 0, width(), height());

    m_proj.setToIdentity();
    m_proj.perspective(45, width() / float(height()), 0.01f, 100.0f);
}

void RenderWindow::setupVertexAttribs()
{
    m_vbo->bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_context->functions()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_context->functions()->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
                                                  (const void *)(36 * 3 * sizeof(GLfloat)));
    m_vbo->release();
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
    if (isExposed()) {
        render();
        if (!m_quickInitialized)
            startQuick(QStringLiteral("Paint.qml"));
    }
}

void RenderWindow::resizeEvent(QResizeEvent *)
{
    // If this is a resize after the scene is up and running, recreate the fbo and the
    // Quick item and scene.
    if (m_rootItem && m_context->makeCurrent(m_offscreenSurface)) {
        delete m_fbo;
        createFbo();
        m_context->doneCurrent();
        updateSizes();
    }
}

void RenderWindow::updateQuick()
{
    qDebug() << "updaatequick";
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

void RenderWindow::render()
{
    if (!m_context->makeCurrent(this))
        return;

    QOpenGLFunctions *f = m_context->functions();
    f->glClearColor(0.0f, 0.1f, 0.25f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_quickReady) {
        f->glFrontFace(GL_CW);
        f->glEnable(GL_CULL_FACE);
        f->glEnable(GL_DEPTH_TEST);

        m_scenemutex->lock();
        f->glBindTexture(GL_TEXTURE_2D, m_fbo->texture());
        m_scenemutex->unlock();

        m_program->bind();
        QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

        if (!m_vao->isCreated())
            setupVertexAttribs();

        static GLfloat angle = 0;
        QMatrix4x4 m;
        m.translate(0, 0, -2);
        m.rotate(90, 0, 0, 1);
        m.rotate(angle, 0.5, 1, 0);
        angle += 0.5f;

        m_program->setUniformValue(m_matrixLoc, m_proj * m);

        // Draw the cube.
        f->glDrawArrays(GL_TRIANGLES, 0, 36);

        m_program->release();
        f->glDisable(GL_DEPTH_TEST);
        f->glDisable(GL_CULL_FACE);
    }

    m_context->swapBuffers(this);
}

void RenderWindow::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "999999999999999999999999999999999999999999"<<e <<  e->button() << e->buttons()<<e->modifiers();
    QMouseEvent mappedEvent(e->type(), e->localPos(), e->button(), e->buttons(), e->modifiers());
    QCoreApplication::sendEvent(m_quickWindow, &mappedEvent);
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *e)
{
    QMouseEvent mappedEvent(e->type(), e->localPos(), e->screenPos(), e->button(), e->buttons(), e->modifiers());
    QCoreApplication::sendEvent(m_quickWindow, &mappedEvent);
}
