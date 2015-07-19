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

BackGroundRenderer::BackGroundRenderer(int port)
    : m_program(0),
      m_vbo(0),
      m_vao(0)
{
    m_mtx = new std::mutex;

    m_imgReceiver = new ImageReceiver(port,m_mtx);
    imgthread = new QThread;
    m_imgReceiver->moveToThread(imgthread);
    QObject::connect(this, SIGNAL(imgstart()), m_imgReceiver, SLOT(run()));
    imgthread->start();
    imgstart();
}

BackGroundRenderer::~BackGroundRenderer()
{
    delete m_program;
    delete m_vbo;
    delete m_vao;
}

void BackGroundRenderer::init(QOpenGLContext* share)
{
    auto *f = share->functions();

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
        "  // gl_FragColor = vec4(1.0,1.0,0.0, 0.5);\n"
        "   gl_FragColor = vec4(texture2D(sampler, v_coord).rgb, 1.0);\n"
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
            -1,1,1,    1,-1,1,     -1,-1,1,
            1,-1,1,   -1,1,1,       1,1, 1
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
        setupVertexAttribs(share);
}

void BackGroundRenderer::resize(int w, int h)
{

}

void BackGroundRenderer::setupVertexAttribs(QOpenGLContext* share)
{

    auto *f = share->functions();

    m_vbo->bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
                                                  (const void *)(6 * 3 * sizeof(GLfloat)));
    m_vbo->release();
}

void BackGroundRenderer::render(QOpenGLContext* share)
{


    auto *f = share->functions();

    f->glFrontFace(GL_CW);


    f->glUseProgram(m_program->programId());
    f->glActiveTexture (GL_TEXTURE0);
    f->glUniform1i     (f->glGetUniformLocation (m_program->programId(), "sampler"), 0);

    int width = 640;
    int height = 480;

    m_mtx->lock();
      m_videoImage = m_imgReceiver->getImageData();
    m_mtx->unlock();

        QImage image(m_videoImage.data,640,480,QImage::Format_ARGB32);
        if(m_videotex != nullptr) delete m_videotex;
        m_videotex = new QOpenGLTexture(image.mirrored());
        m_videotex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_videotex->setMagnificationFilter(QOpenGLTexture::Linear);

    m_videotex->bind();


    QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

    if (!m_vao->isCreated())
        setupVertexAttribs(share);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);

    qDebug() << "backcomen";

}
