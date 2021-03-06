#include "backgroundrenderer.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOffscreenSurface>
#include <QWindow>
#include <QVector3D>


#include "eigenutil.h"
#include "glutil.h"

BackGroundRenderer::BackGroundRenderer(int port, std::shared_ptr<QOpenGLContext>& share)
    : m_program(nullptr),
      m_vbo(nullptr),
      m_vao(nullptr),
      m_videotex(nullptr)
{
    m_mtx = new QMutex();

    m_context = share;

    m_imgReceiver = new ImageReceiver(port,m_mtx);
    imgthread = new QThread;
    m_imgReceiver->moveToThread(imgthread);
    QObject::connect(this, SIGNAL(imgstart()), m_imgReceiver, SLOT(run()));
    imgthread->start();
    imgstart();

    /*
    m_rwindow = new RenderWindow(m_mtx, "web.qml");
    m_rwindow->init();
    m_rwindow->resize(960, 1080);
    m_rwindow->show();
*/
}

BackGroundRenderer::~BackGroundRenderer()
{
    delete m_program;
    delete m_vbo;
    delete m_vao;
}

void BackGroundRenderer::init()
{
    auto *f = m_context->functions();


    static const char *vertexShaderSource =
        "attribute highp vec4 vertex;\n"
        "attribute lowp vec2 coord;\n"
        "varying lowp vec2 v_coord;\n"
        ""
        "void main() {\n"
        "   v_coord = coord;\n"
        "   gl_Position = vertex;\n"
        "}\n";
    static const char *fragmentShaderSource =
        "varying lowp vec2 v_coord;\n"
        "uniform sampler2D sampler;\n"
        "void main() {\n"
        "   //gl_FragColor = vec4(1.0,1.0,0.0, 0.5);\n"
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
    QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();

    GLfloat v[] = {
            -1,-1,1,    1,1,1,     1,-1,1,
            1,1,1,   -1,-1,1,       -1,1, 1
        };
  /*
    GLfloat v[] = {
            -1,1,1,    1,-1,1,     -1,-1,1,
            1,-1,1,   -1,1,1,       1,1, 1
        };
*/
    GLfloat texCoords[] = {
        0.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
        1.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f,
    };
  /*
    GLfloat texCoords[] = {
        0.0f,0.85f, 1.0f,0.15f, 0.0f,0.15f,
        1.0f,0.15f, 0.0f,0.85f, 1.0f,0.85f,
    };
*/
    const int vertexCount = 6;
    m_vbo->allocate(sizeof(GLfloat) * vertexCount * 5);
    m_vbo->write(0, v, sizeof(GLfloat) * vertexCount * 3);
    m_vbo->write(sizeof(GLfloat) * vertexCount * 3, texCoords, sizeof(GLfloat) * vertexCount * 2);
    m_vbo->release();


    if (m_vao->isCreated())
        setupVertexAttribs();
}



void BackGroundRenderer::setupVertexAttribs()
{

    auto *f = m_context->functions();

    m_vbo->bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
                                                  (const void *)(6 * 3 * sizeof(GLfloat)));
    m_vbo->release();
}

void BackGroundRenderer::render()
{


    auto *f = m_context->functions();

    f->glFrontFace(GL_CW);


    f->glUseProgram(m_program->programId());
    f->glActiveTexture (GL_TEXTURE0);
    f->glUniform1i     (f->glGetUniformLocation (m_program->programId(), "sampler"), 0);

    int width = 640;
    int height = 480;
/*
    m_mtx->lock();
      m_videoImage = m_imgReceiver->getImageData();
       m_videotex.reset(new QOpenGLTexture(m_rwindow->qmlimage.mirrored()));
       m_videotex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
       m_videotex->setMagnificationFilter(QOpenGLTexture::Linear);
*/
   // m_mtx->unlock();


    m_mtx->lock();
    m_videoImage = m_imgReceiver->getImageData();
        QImage image = QRR::CV::cvMatToQImage(m_videoImage);

        m_videotex.reset(new QOpenGLTexture(image.mirrored()));
        m_videotex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_videotex->setMagnificationFilter(QOpenGLTexture::Linear);
    m_mtx->unlock();

    m_videotex->bind();


    QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

    if (!m_vao->isCreated())
        setupVertexAttribs();


    f->glDrawArrays(GL_TRIANGLES, 0, 6);


}
