
#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>


#include "mainwindow.h"

#include <iostream>
#include <tuple>

template < typename T >
constexpr T sqrt( T s )
{ return sqrt_aux( s, s/2.0, s ) ; }


auto main (int argc, char *argv[]) -> int
{
    QApplication a(argc, argv);

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);

    if(QCoreApplication::arguments().contains(QStringLiteral("--multisample")))
           fmt.setSamples(4);

    if(QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"))){
        fmt.setVersion(3,2);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    }

    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow mainWindow;
    //mainWindow.resize(mainWindow.sizeHint());
 /*   int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = mainWindow.width() * mainWindow.height();
    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        mainWindow.show();
    else
        mainWindow.showMaximized();
*/

    mainWindow.show();

    QMainWindow secondwindow_;
    secondwindow_.setCentralWidget((QWidget*)mainWindow.subadmin);
    secondwindow_.show();



    int test = 0b0011;


    std::cout << test << std::endl;

    return a.exec();
}
