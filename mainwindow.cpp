
#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow()
{
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuWindow = menuBar->addMenu(tr("&Window"));
    QAction *addNew = new QAction(menuWindow);
   /* addNew->setText(tr("Add new"));
    menuWindow->addAction(addNew);
    connect(addNew, SIGNAL(triggered()), this, SLOT(onAddNew()));
    setMenuBar(menuBar);
*/
    setTree();

    onAddNew();

    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::setTree(){
    //treeview = new QTreeView();
    //QWidget *widget = new QWidget();
    //setCentralWidget(widget);
}

void MainWindow::onAddNew()
{


    //    if (!centralWidget()){

    QDockWidget *dock = new QDockWidget(tr("gamewindow"), this);
    adminwidget = new AdminWidget(dock);

    //    dock->setWidget(new QLabel("\aaa", dock));
    //   dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    // addDockWidget(Qt::RightDockWidgetArea, dock);
    setCentralWidget(adminwidget);
    //  }
    //else QMessageBox::information(0, tr("Cannot add new window"), tr("Already occupied. Undock first."));


    subadmin = new QLabel(nullptr);
    subadmin->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    subadmin->setWordWrap(true);

    //subadmin->show();

    QObject::connect(adminwidget, SIGNAL(capturesignal()), this , SLOT(capturewidget()));
}



void MainWindow::createActions(){

}

void MainWindow::createDockWindows(){

}

#include <QGuiApplication>
#include <QScreen>

void MainWindow::capturewidget(){
/*
    QPixmap screenshot(QSize( 100, 100 ));
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen){
        screenshot = screen->grabWindow(0);
    }
    subadmin->setPixmap(screenshot);
*/
}
