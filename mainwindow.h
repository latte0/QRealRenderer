#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTreeView>
#include "adminwidget.h"

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

private slots:
    void onAddNew();
    void capturewidget();

private:
    void setTree();
    void createActions();
    void createDockWindows();
};

#endif // MAINWINDOW_H

