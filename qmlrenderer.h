#ifndef QMLRENDER_H
#define QMLRENDER_H

#include "windowrenderer.h"
#include "renderwindow.h"

class RenderWindow;

class qmlRenderer : public WindowRenderer
{
public:
    qmlRenderer();
    virtual ~qmlRenderer();

    virtual void inittex(QOpenGLContext * share);
    virtual void bindTex();
    virtual void collide(Eigen::Vector3f top);
    virtual void update();


    RenderWindow *m_rwindow;
    QOpenGLTexture *m_qmltex;

    float touch_x = 0, touch_y = 0;
};

#endif // QMLRENDER_H
