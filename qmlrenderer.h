#ifndef QMLTEXTURE_H
#define QMLTEXTURE_H

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


class qmltexture : public QObject :public
{
    Q_OBJECT

public:
    qmltexture();
    ~qmltexture();

    void initqml(const QString &filename);
    void updateQuick();
    void run();

public:
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
};

#endif // QMLTEXTURE_H
