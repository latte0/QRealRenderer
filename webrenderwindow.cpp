#include "webrenderwindow.h"

WebRenderWindow::WebRenderWindow(QMutex* mutex, const QString &filename)
    :RenderWindow(mutex,filename)
{
    /*
   m_qmlEngine->rootContext()->setContextProperty("utils", new Utils(this));

   QMetaObject::invokeMethod(m_qmlEngine->rootObjects().first(), "load", Q_ARG(QVariant,m_qmlEngine->startupUrl()));
   */
}

WebRenderWindow::~WebRenderWindow()
{}
