#ifndef COPYWINDOW_H
#define COPYWINDOW_H


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
#include <QQmlContext>
#include <QOpenGLTexture>

#include <QMutex>
#include <QTimer>

#include "boardobject.h"


class CopyWindow : public QWindow ,BoardObject
{
    Q_OBJECT

public:
    CopyWindow(QMutex *mutex ,QImage *copyimage);
    ~CopyWindow();

    void render();

    void setupVertexAttribs();

protected:
    void exposeEvent(QExposeEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

private slots:
    void copyupdate();
    void copyinit();

private:
    QOpenGLContext *m_context;
    QOffscreenSurface *m_offscreenSurface;
    QQuickRenderControl *m_renderControl;

    QOpenGLFramebufferObject *m_fbo;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLVertexArrayObject *m_vao;

    QOpenGLTexture *m_scenetex;
    QImage *m_copyimage;


    QMutex *m_scenemutex;

    QTimer m_updateTimer;
    QTimer m_initTimer;

    bool m_Initialized;
    int m_matrixLoc;
};

#endif // COPYWINDOW_H
