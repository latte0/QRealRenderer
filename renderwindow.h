#ifndef _RENDERWINDOW_H_
#define _RENDERWINDOW_H_

#include <QWindow>
#include <QMatrix4x4>
#include <QTimer>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOffscreenSurface>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QCoreApplication>


#include <QtWebEngine/QtWebEngine>


#include <QMutex>
#include "scenerender.h"


QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLFramebufferObject)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)
QT_FORWARD_DECLARE_CLASS(QOffscreenSurface)
QT_FORWARD_DECLARE_CLASS(QQuickRenderControl)
QT_FORWARD_DECLARE_CLASS(QQuickWindow)
QT_FORWARD_DECLARE_CLASS(QQmlEngine)
QT_FORWARD_DECLARE_CLASS(QQmlComponent)
QT_FORWARD_DECLARE_CLASS(QQuickItem)


class RenderWindow : public QWindow
{
    Q_OBJECT

public:
    RenderWindow(QMutex* mutex, const QString &filename);
    ~RenderWindow();

    QImage qmlimage;
    QMutex *m_scenemutex;


    virtual void init();
    void updateQuick();
    QOpenGLContext * getContext();
    QOpenGLFramebufferObject *getQmlFbo();

    int getQmlWidth();
    int getQmlHeight();
    QSize getQmlSize();

        void startQuick(const QString &filename);


    QQuickWindow *m_quickWindow;
    QOpenGLVertexArrayObject *m_vao;

    QQuickItem *m_rootItem;

protected:
    void exposeEvent(QExposeEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;


public slots:
    void run();
    void createFbo();
    void destroyFbo();
    void requestUpdate();



private:


    QOpenGLContext *m_context;
    QOffscreenSurface *m_offscreenSurface;
    QQuickRenderControl *m_renderControl;

    QString m_filename;

    QQmlEngine *m_qmlEngine;
    QQmlComponent *m_qmlComponent;

    QOpenGLFramebufferObject *m_fbo;


    bool m_quickInitialized;
    bool m_quickReady;
    int m_matrixLoc;
    QTimer m_updateTimer;

    int m_qmlwidth, m_qmlheight;
};

#endif
