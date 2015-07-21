#ifndef QMLRENDER_H
#define QMLRENDER_H

#include "windowrenderer.h"
#include "renderwindow.h"

class RenderWindow;

class qmlRenderer : public WindowRenderer
{
public:
    qmlRenderer();
    ~qmlRenderer();

    virtual void inittex();
    virtual void bindTex();
    virtual void collide(Eigen::Vector3f top);


    RenderWindow *m_rwindow;

    QOpenGLTexture *m_qmltex;

};

#endif // QMLRENDER_H
