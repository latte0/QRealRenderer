#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QMatrix4x4>
#include <QTimer>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>

#include <QOffscreenSurface>
#include <QScreen>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickRenderControl>


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

#include "scenerender.h"

class MainWindow;


class AdminWidget : public QWidget
{
   Q_OBJECT
public:
    explicit AdminWidget(QWidget *parent = 0);
    ~AdminWidget();

private:
    MainWindow *mainWindow;
    SceneRender *gameapp;
    QTimer *timer;

signals:
    void capturesignal();

public slots:
    void capturetimeout();

private slots:
    void render();
    void updateQuick();
    void run();
    void createFbo();
    void destroyFbo();
    void requestUpdate();

private:
    QWindow *m_window;

    void startQuick(const QString &filename);
    void setupVertexAttribs();
    void updateSizes();

    void expose();

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
    QMatrix4x4 m_proj;
    QTimer m_updateTimer;
};

#endif // ADMINWIDGET_H
