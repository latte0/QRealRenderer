#ifndef WEBRENDERWINDOW_H
#define WEBRENDERWINDOW_H
#include "renderwindow.h"

class WebRenderWindow : public  RenderWindow
{

public:
    WebRenderWindow(QMutex* mutex, const QString &filename);
    ~WebRenderWindow();
};

#endif // WEBRENDERWINDOW_H
