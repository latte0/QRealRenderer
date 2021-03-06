
#include "windowrenderer.h"
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

#include "math.h"

WindowRenderer::WindowRenderer(std::shared_ptr<QOpenGLContext>& share)
    : m_program(0),
      m_vbo(0),
      m_vao(0),
      m_matrixLoc(0),
      RectangleObject(),
      m_z(0.0f),
      m_scale(0.5f),
      m_rightrot(0.0f),
      m_uprot(0.0f),
      m_handtouch(true),
      m_touched(false),
      m_pressed(false),
      touchdistance(6.0),
      m_mutex(new QMutex()),
      m_context(share)

{

}

WindowRenderer::~WindowRenderer()
{
    delete m_program;
    delete m_vbo;
    delete m_vao;

}

void WindowRenderer::inittex(){

}

void WindowRenderer::init( const QString &filename)
{

    m_filename = filename;
    inittex();




    static const char *vertexShaderSource =
        "attribute highp vec4 vertex;\n"
        "attribute lowp vec2 coord;\n"
        "varying lowp vec2 v_coord;\n"
        "uniform highp mat4 matrix;\n"
        "uniform float aspect;\n"
        "void main() {\n"
        "   v_coord = coord;\n"
        "   gl_Position = matrix * vec4(vertex.x, vertex.y / aspect, vertex.z, vertex.w);\n"
        "}\n";
    static const char *fragmentShaderSource =
        "varying lowp vec2 v_coord;\n"
        "uniform sampler2D sampler;\n"
        "void main() {\n"
        "   //gl_FragColor = vec4(1.0,1.0,0.0, 0.5);\n"
        "   vec4 riftcolor = texture2D(sampler,  v_coord);"
        "   gl_FragColor = vec4(riftcolor.b, riftcolor.g, riftcolor.r, 0);\n"
        "}\n";


    qDebug() <<" before program link";

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("coord", 1);
    m_program->link();
    m_matrixLoc = m_program->uniformLocation("matrix");
    m_aspectLoc = m_program->uniformLocation("aspect");

    qDebug() << "after program link";

    m_vao = new QOpenGLVertexArrayObject;
    m_vao->create();
    QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();

    GLfloat v[] = {
            -1,1,0,    1,-1 ,0,     -1,-1,0,
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

}

void WindowRenderer::resize(int w, int h)
{

}

void WindowRenderer::setupVertexAttribs()
{

    auto *f = m_context->versionFunctions<QOpenGLFunctions_3_3_Core>();

    m_vbo->bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
                                                  (const void *)(6 * 3 * sizeof(GLfloat)));
    m_vbo->release();
}

void WindowRenderer::setCondition(float scale, Eigen::Vector3f pos, float uprot, float rightrot, bool handtouch)
{
    m_scale = scale;
    m_position = pos;
    m_uprot = uprot;
    m_rightrot = rightrot;


    //euler rotation
    Eigen::Vector3f mzbasis(cos(m_rightrot * 3.1415/180),0,-sin(m_rightrot * 3.1415/180));

    m_basis = QRRUtil::MakeEulerYRotationMat(m_rightrot * 3.1415/180 );
    m_basis = QRRUtil::MakeEulerRotationMat(mzbasis, m_uprot * 3.1415/180 ) * m_basis;

    m_handtouch = handtouch;
}

void WindowRenderer::collide(Eigen::Vector3f top){

}

void WindowRenderer::update(){

}

void WindowRenderer::objectupdate( Eigen::Matrix4f mat, Eigen::Vector3f top){

    QMatrix4x4 psmat;

    psmat.scale(m_scale,m_scale);

    m_smat = psmat;


    Eigen::Matrix4f esmat;
    esmat << m_scale, 0,0,0,
            0,m_scale,0,0,
            0,0,m_scale,0,
            0,0,0,1.0;


    initRecpos();
    calcRecpos(QRRUtil::MakeTransform(m_position) * m_basis * esmat);
    createWorld();


    auto eToqMat = [](const Eigen::Matrix4f& emat) -> QMatrix4x4{
            QMatrix4x4 qmat(emat(0,0),emat(0,1),emat(0,2),emat(0,3),
                                emat(1,0),emat(1,1),emat(1,2),emat(1,3),
                                emat(2,0),emat(2,1),emat(2,2),emat(2,3),
                                emat(3,0),emat(3,1),emat(3,2),emat(3,3));
            return  qmat;
    };

    m_qworld = eToqMat(m_world);
    m_qmat = eToqMat(mat);


    m_rightVec = m_recPositions[1] - m_recPositions[0];
    m_downVec = m_recPositions[2] - m_recPositions[0];

//    if(m_handtouch == false){     m_vertVec = mousepos; }
     m_vertVec = m_rightVec.cross(m_downVec);


    collide(top);

}

void WindowRenderer::bindTex(){

}

void WindowRenderer::render( Eigen::Matrix4f mat, Eigen::Vector3f top)
{

    auto *f = m_context->versionFunctions<QOpenGLFunctions_3_3_Core>();

    f->glFrontFace(GL_CW);

    f->glUseProgram(m_program->programId());
    f->glActiveTexture (GL_TEXTURE0);
    f->glUniform1i     (f->glGetUniformLocation (m_program->programId(), "sampler"), 0);

    bindTex();

   QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);


        objectupdate(mat, top);
        update();

        if (!m_vao->isCreated())
            setupVertexAttribs();

        m_program->setUniformValue(m_matrixLoc, m_qmat * m_qworld * m_smat);
        m_program->setUniformValue(m_aspectLoc, getAspect());

        f->glDrawArrays(GL_TRIANGLES, 0, 6);

}
