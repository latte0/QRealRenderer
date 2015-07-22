#include "copywindow.h"

CopyWindow::CopyWindow(QMutex *mutex ,QImage *copyimage)
    :m_fbo(0),
      m_program(0),
      m_vbo(0),
      m_scenemutex(nullptr),
      m_scenetex(nullptr),
      m_Initialized(false)
{
    setSurfaceType(QSurface::OpenGLSurface);

    m_scenemutex = mutex;
    m_copyimage = copyimage;

    QSurfaceFormat format;

    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);
    setFormat(format);

    m_context = new QOpenGLContext;
    m_context->setFormat(format);
    m_context->create();

    m_updateTimer.setSingleShot(true);
    m_updateTimer.setInterval(4);
    m_updateTimer.start();
    connect(&m_updateTimer, &QTimer::timeout, this, &CopyWindow::copyinit);


}

CopyWindow::~CopyWindow()
{

}

void CopyWindow::copyinit()
{

    qDebug() << "copy window init";

    if (!m_context->makeCurrent(this))
        return;


    static const char *vertexShaderSource =
        "attribute highp vec4 vertex;\n"
        "attribute lowp vec2 coord;\n"
        "varying lowp vec2 v_coord;\n"
        "void main() {\n"
        "   v_coord = coord;\n"
        "   gl_Position = vertex;\n"
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


    m_vao = new QOpenGLVertexArrayObject;
    m_vao->create();
    m_vao->bind();

    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();

    GLfloat v[] = {
            -1,1,0,    1,-1,0,     -1,-1,0,
            1,-1,0,   -1,1,0,       1,1, 0
        };

    GLfloat texCoords[] = {
        0.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
        1.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f,
    };

    const int vertexCount = 6;
    m_vbo->allocate(sizeof(GLfloat) * vertexCount * 5);
    m_vbo->write(0, v, sizeof(GLfloat) * vertexCount * 3);
    m_vbo->write(sizeof(GLfloat) * vertexCount * 3, texCoords, sizeof(GLfloat) * vertexCount * 2);
    m_vbo->release();

    if (m_vao->isCreated())
        setupVertexAttribs();

    m_updateTimer.setSingleShot(true);
    m_updateTimer.setInterval(4);
    m_updateTimer.start();
    connect(&m_updateTimer, &QTimer::timeout, this, &CopyWindow::copyupdate);


    m_vao->release();

    m_context->doneCurrent();


    qDebug() << "finish copywin dow iit";
    m_Initialized = true;



}

void CopyWindow::setupVertexAttribs(){
    m_vbo->bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_context->functions()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_context->functions()->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
                                                  (const void *)(6 * 3 * sizeof(GLfloat)));
    m_vbo->release();
}


void CopyWindow::copyupdate(){
    render();
}

void CopyWindow::render()
{

    if (!m_context->makeCurrent(this))
        return;


    QOpenGLFunctions *f = m_context->functions();


    f->glClearColor(0.0f, 0.1f, 0.25f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    f->glViewport(0,0,1920,1080);

        f->glFrontFace(GL_CW);
        f->glEnable(GL_CULL_FACE);
        f->glEnable(GL_DEPTH_TEST);

      //  m_scenemutex->lock();
            if(m_scenetex != nullptr) delete m_scenetex;
            m_scenetex = new QOpenGLTexture(m_copyimage->mirrored());
            m_scenetex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            m_scenetex->setMagnificationFilter(QOpenGLTexture::Linear);
      //  m_scenemutex->unlock();

       m_scenetex->bind();


        m_program->bind();

        QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);



        if (!m_vao->isCreated())
            setupVertexAttribs();

        f->glDrawArrays(GL_TRIANGLES, 0, 6);

        m_program->release();

        f->glDisable(GL_DEPTH_TEST);
        f->glDisable(GL_CULL_FACE);

    m_context->swapBuffers(this);


    m_updateTimer.start();
}


void CopyWindow::exposeEvent(QExposeEvent *)
{
}

void CopyWindow::resizeEvent(QResizeEvent *)
{


}

void CopyWindow::mousePressEvent(QMouseEvent *e)
{

}

void CopyWindow::mouseReleaseEvent(QMouseEvent *e)
{

}

