#include "qmlrenderer.h"
#include "eigenutil.h"

#include "environmentreader.h"

QmlRenderer::QmlRenderer()
    : m_rootItem(0),
      m_fbo(0),
      m_program(0),
      m_vbo(0),
      m_quickInitialized(false),
      m_quickReady(false)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);
    m_context = new QOpenGLContext;
    m_context->setFormat(format);
    m_context->create();

    m_renderControl = new QQuickRenderControl();

    m_quickWindow = new QQuickWindow(m_renderControl);

    m_qmlEngine = new QQmlEngine;
    if (!m_qmlEngine->incubationController())
        m_qmlEngine->setIncubationController(m_quickWindow->incubationController());

    QObject::connect(m_quickWindow, &QQuickWindow::sceneGraphInitialized, this, &QmlRenderer::createFbo);
    QObject::connect(m_quickWindow, &QQuickWindow::sceneGraphInvalidated, this, &QmlRenderer::destroyFbo);
    QObject::connect(m_renderControl, &QQuickRenderControl::renderRequested, this, &QmlRenderer::requestUpdate);
    QObject::connect(m_renderControl, &QQuickRenderControl::sceneChanged, this, &QmlRenderer::requestUpdate);
}

QmlRenderer::~QmlRenderer()
{

}

void QmlRenderer::createFbo()
{
    // The scene graph has been initialized. It is now time to create an FBO and associate
    // it with the QQuickWindow.
    m_fbo = new QOpenGLFramebufferObject(960,1080, QOpenGLFramebufferObject::CombinedDepthStencil);
    m_quickWindow->setRenderTarget(m_fbo);
}

void QmlRenderer::destroyFbo()
{
    delete m_fbo;
    m_fbo = 0;
}

void QmlRenderer::setupVertexAttribs()
{
    m_vbo->bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_context->functions()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_context->functions()->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,(const void *)(36 * 3 * sizeof(GLfloat)));
    m_vbo->release();
}


void QmlRenderer::initqml(const QString &filename)
{

}

void QmlRenderer::update(Player *theplayer){
    this->updateQuick();
    this->render(theplayer);
}

void QmlRenderer::run()
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
        "   gl_FragColor = vec4(texture2D(sampler, v_coord).rgb, 1.0);\n"
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

    using namespace cpplinq;

    GLfloat v[] = {
        -0.5, 0.5, 0.5, 0.5,-0.5,0.5,-0.5,-0.5,0.5,
        0.5, -0.5, 0.5, -0.5,0.5,0.5,0.5,0.5,0.5,
    };

   // from_array(v) >> for_each([](GLfloat *f){*f = *f+ 10;});


    GLfloat texCoords[] = {
        0.0f,0.0f, 1.0f,1.0f, 1.0f,0.0f,
        1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,

    };

    const int vertexCount = 6;
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

void QmlRenderer::updateQuick()
{
    if( !m_context->makeCurrent(m_offscreenSurface));
    m_renderControl->polishItems();
    m_renderControl->sync();
    m_renderControl->render();

    m_quickWindow->resetOpenGLState();

    m_quickReady = true;
}

void QmlRenderer::render(Player *theplayer){
    makeCurrent();
}

void QmlRenderer::requestUpdate(){

}

void QmlRenderer::expose(){

}
