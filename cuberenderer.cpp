
#include "cuberenderer.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOffscreenSurface>
#include <QWindow>
#include <QVector3D>

#include "external/external_opengl/glm/glm.hpp"
#include "external/external_opengl/glm/gtc/type_ptr.hpp"

#include "external/external_opengl/FreeImage/FreeImage.h"
#include "external/cppitertools/itertools.hpp"
#include "external/external_opengl/glm/glm.hpp"
#include "external/external_opengl/glm/gtc/matrix_transform.hpp"

#include "eigenutil.h"

CubeRenderer::CubeRenderer()
    : m_program(0),
      m_vbo(0),
      m_vao(0),
      m_matrixLoc(0)
{

}

CubeRenderer::~CubeRenderer()
{
    delete m_program;
    delete m_vbo;
    delete m_vao;
}

void CubeRenderer::init(QOpenGLContext* share)
{
    auto *f = share->functions();

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
        "   //gl_FragColor = vec4(1.0,1.0,0.0, 0.5);\n"
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
    QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();

    GLfloat v[] = {
        -10,10,-1,    10,-10,-1,     -10,-10,-1,
        10,-10,-1,   -10,10,-1,       10,10, -1
    };

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
        setupVertexAttribs(share);
}

void CubeRenderer::resize(int w, int h)
{

}

void CubeRenderer::setupVertexAttribs(QOpenGLContext* share)
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

void CubeRenderer::render(QOpenGLContext* share, Eigen::Matrix4f mat, QOpenGLTexture *qmltex)
{

    auto *f = share->functions();

    f->glFrontFace(GL_CW);

    f->glUseProgram(m_program->programId());
    f->glActiveTexture (GL_TEXTURE0);
    f->glUniform1i     (f->glGetUniformLocation (m_program->programId(), "sampler"), 0);

        qmltex->bind();

        QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

        QMatrix4x4 qmat(mat(0,0),mat(0,1),mat(0,2),mat(0,3),
                        mat(1,0),mat(1,1),mat(1,2),mat(1,3),
                        mat(2,0),mat(2,1),mat(2,2),mat(2,3),
                        mat(3,0),mat(3,1),mat(3,2),mat(3,3));

        if (!m_vao->isCreated())
            setupVertexAttribs(share);

        static GLfloat angle = 0;



        m_program->setUniformValue(m_matrixLoc, qmat);

        f->glDrawArrays(GL_TRIANGLES, 0, 6);

       // f->glFlush();
}
