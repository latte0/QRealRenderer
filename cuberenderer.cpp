
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
      m_matrixLoc(0),
      m_qmltex(nullptr)
{

    m_positions[0] = QRRUtil::EigenVector3fMake(-1.0, 1.0, 0.0);
    m_positions[1] = QRRUtil::EigenVector3fMake(1.0, 1.0, 0.0);
    m_positions[2] = QRRUtil::EigenVector3fMake(-1.0, -1.0, 0.0);
    m_positions[3] = QRRUtil::EigenVector3fMake(1.0, -1.0, 0.0);
    m_centerpos = QRRUtil::EigenVector3fMake(0.0, 0.0, 0.0);


    m_mutex = new QMutex();



}

CubeRenderer::~CubeRenderer()
{
    delete m_program;
    delete m_vbo;
    delete m_vao;
}

void CubeRenderer::init(QOpenGLContext* share, const QString &filename)
{

        m_rwindow = new RenderWindow(m_mutex,filename);

    m_rwindow->init();

    m_rwindow->resize(960, 1080);
    m_rwindow->show();

    m_filename = filename;

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
    QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

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

void CubeRenderer::render(QOpenGLContext* share, Eigen::Matrix4f mat, Eigen::Vector3f top,  Eigen::Vector3f mousepos)
{

    auto *f = share->functions();

    f->glFrontFace(GL_CW);

    f->glUseProgram(m_program->programId());
    f->glActiveTexture (GL_TEXTURE0);
    f->glUniform1i     (f->glGetUniformLocation (m_program->programId(), "sampler"), 0);

    m_mutex->lock();
        if(m_qmltex != nullptr) delete m_qmltex;
        m_qmltex = new QOpenGLTexture(m_rwindow->qmlimage.mirrored());
        m_qmltex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_qmltex->setMagnificationFilter(QOpenGLTexture::Linear);
    m_mutex->unlock();

    m_qmltex->bind();


        QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

        QMatrix4x4 qmat(mat(0,0),mat(0,1),mat(0,2),mat(0,3),
                        mat(1,0),mat(1,1),mat(1,2),mat(1,3),
                        mat(2,0),mat(2,1),mat(2,2),mat(2,3),
                        mat(3,0),mat(3,1),mat(3,2),mat(3,3));

        QMatrix4x4 s;
        s.scale(m_scale, m_scale);
        s.translate(0.0f,0.0f,m_z);

        m_rightrot=0;

        Eigen::Matrix4f rightrotmat = QRRUtil::MakeEulerYRotationMat(m_rightrot * 3.1415/180 );


        QMatrix4x4 qrightrotmat(rightrotmat(0,0),rightrotmat(0,1),rightrotmat(0,2),rightrotmat(0,3),
                        rightrotmat(1,0),rightrotmat(1,1),rightrotmat(1,2),rightrotmat(1,3),
                        rightrotmat(2,0),rightrotmat(2,1),rightrotmat(2,2),rightrotmat(2,3),
                        rightrotmat(3,0),rightrotmat(3,1),rightrotmat(3,2),rightrotmat(3,3));


        Eigen::Vector3f mzbasis(cos(m_rightrot * 3.1415/180),0,-sin(m_rightrot * 3.1415/180));



        Eigen::Matrix4f uprotmat = QRRUtil::MakeEulerRotationMat(mzbasis,m_uprot * 3.1415/180 );


        QMatrix4x4 quprotmat(uprotmat(0,0),uprotmat(0,1),uprotmat(0,2),uprotmat(0,3),
                        uprotmat(1,0),uprotmat(1,1),uprotmat(1,2),uprotmat(1,3),
                        uprotmat(2,0),uprotmat(2,1),uprotmat(2,2),uprotmat(2,3),
                        uprotmat(3,0),uprotmat(3,1),uprotmat(3,2),uprotmat(3,3));




        m_positions[0] = QRRUtil::EigenVector3fMake(-1.0, 1.0, 0.0);
        m_positions[1] = QRRUtil::EigenVector3fMake(1.0, 1.0, 0.0);
        m_positions[2] = QRRUtil::EigenVector3fMake(-1.0, -1.0, 0.0);
        m_positions[3] = QRRUtil::EigenVector3fMake(1.0, -1.0, 0.0);
        m_centerpos = QRRUtil::EigenVector3fMake(0.0, 0.0, 0.0);

        for(int i =0;i < 4;i++){
            m_positions[i] = m_positions[i] * m_scale;
            m_positions[i].z() = m_positions[i].z() + m_z;
            m_positions[i] = QRRUtil::MakeEulerYRotationVec(m_positions[i], m_rightrot * 3.1415/180 );
            m_positions[i] = QRRUtil::MakeEulerRotationVec(mzbasis,m_positions[i], m_uprot * 3.1415/180 );
        }

        m_centerpos.z() = m_centerpos.z() + m_z;
        m_centerpos = QRRUtil::MakeEulerYRotationVec(m_centerpos, m_rightrot * 3.1415/180 );
        m_centerpos = QRRUtil::MakeEulerRotationVec(mzbasis,m_centerpos, m_uprot * 3.1415/180 );


        m_rightVec = m_positions[1] - m_positions[0];
        m_downVec = m_positions[2] - m_positions[0];
        m_suiVec = mousepos;

        if(getH(top) < 5){
            std::cout << "height " << getH(top) << std::endl;
            Eigen::Vector2f pos = calcPos(top);
            qDebug() << "10 ika" ;
            std::cout << pos << std::endl;
/*
            QMouseEvent mappedEvent(QEvent::MouseButtonPress,  QPointF(1000* pos.x(), 1000 * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
            QCoreApplication::sendEvent(m_rwindow->m_quickWindow, &mappedEvent);
*/
        }

        if (!m_vao->isCreated())
            setupVertexAttribs(share);

        m_program->setUniformValue(m_matrixLoc, qmat * quprotmat * qrightrotmat* s);

        f->glDrawArrays(GL_TRIANGLES, 0, 6);

}
