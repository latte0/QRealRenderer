#ifndef FBXRENDERER_H
#define FBXRENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFramebufferObject>

#include <QPoint>
#include <QMouseEvent>
#include <QTimer>
#include <QFile>
#include <QUdpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QMessageBox>

#include <QOffscreenSurface>
#include <QScreen>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickRenderControl>

#include <QWindow>
#include <QMatrix4x4>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>


#include <math.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <highgui.hpp>
#include <core.hpp>
#include <imgproc.hpp>


#include "fbxloader.h"
#include "handinfo.h"

#include "external/external_opengl/FreeImage/FreeImage.h"
#include "external/cppitertools/itertools.hpp"
#include "external/external_opengl/glm/glm.hpp"

#include "glutil.h"
#include "fbxstruct.h"

#include "renderactor.h"


#define MAX_BONE_COUNT 128

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


class FbxRenderer : public RenderActor
{
public:
    FbxRenderer();

    GLuint loadTexture (QOpenGLContext *share, const std::string &filename);



    virtual void update( UniformVs uniformvs);
    virtual void init(QOpenGLContext *share, const std::string &filename);
    virtual void render(QOpenGLContext *share,  UniformVs uniformvs);

    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *hand_program;


    GLuint m_uniformBufferVs;
    UniformVs m_uniformVs;
    GLint uniformsSkin[NUM_UNIFORMS];
    GLint uniformsCloth[NUM_UNIFORMS];


    fbxLoader m_fbxLoader;
    std::map<std::string, GLuint> m_textureDictionary;
    std::vector<AppMaterial> m_materialList;
    std::vector<AppMesh> m_meshlist;

    float m_frame = 0;
};

#endif // FBXRENDERER_H
