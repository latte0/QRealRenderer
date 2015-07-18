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

private:


};

#endif // ADMINWIDGET_H
