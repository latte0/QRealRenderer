#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFramebufferObject>

#include <QMainWindow>
#include <QDockWidget>
#include <QTreeView>
#include "adminwidget.h"
#include "scenario.h"

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    AdminWidget *adminwidget;
    QLabel *subadmin;
    QTreeView *treeview;

    std::unique_ptr<Scenario> m_scenario;
/*
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;
*/

private slots:
    void onAddNew();
    void capturewidget();

private:
    void setTree();
    void createActions();
    void createDockWindows();
};

#endif // MAINWINDOW_H

