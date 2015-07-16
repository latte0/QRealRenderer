#ifndef QMLTEXTURE_H
#define QMLTEXTURE_H

#include "actor.h"
#include "player.h"
#include "external/cpplinq.hpp"

#include <QWindow>
#include <QMatrix4x4>
#include <QTimer>
#include <QString>


#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOffscreenSurface>
#include <QQuickRenderControl>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Compatibility>


class QmlRenderer : public QOpenGLWidget
{
    Q_OBJECT
public:
    QmlRenderer();
    ~QmlRenderer();

    void initqml(const QString &filename);

private slots:

    void update(Player *theplayer);

    void render(Player *theplayer);
    void updateQuick();
    void run();

    void createFbo();
    void destroyFbo();
    void requestUpdate();

    void expose();

private:
    void setupVertexAttribs();


public:

    QOpenGLContext *m_context;
    QOffscreenSurface *m_offscreenSurface;
    QQuickRenderControl *m_renderControl;
    QQuickWindow *m_quickWindow;
    QQmlEngine *m_qmlEngine;
    QQmlComponent *m_qmlComponent;
    QQuickItem *m_rootItem;
    QOpenGLFramebufferObject *m_fbo;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLVertexArrayObject *m_vao;
    bool m_quickInitialized;
    bool m_quickReady;
    int m_matrixLoc;

};

#endif // QMLTEXTURE_H
