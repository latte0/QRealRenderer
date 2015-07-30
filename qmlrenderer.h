#ifndef QMLRENDER_H
#define QMLRENDER_H

#include <tuple>

#include "windowrenderer.h"
#include "renderwindow.h"

class RenderWindow;

class qmlRenderer : public WindowRenderer
{
public:
    qmlRenderer(std::shared_ptr<QOpenGLContext>& share);
    virtual ~qmlRenderer();

    virtual void inittex();
    virtual void bindTex();
    virtual void collide(Eigen::Vector3f top);
    virtual void update();

    QSize getQmlSize();

    RenderWindow *m_rwindow;
    std::unique_ptr<QOpenGLTexture>  m_qmltex;

    float touch_x = 0, touch_y = 0;
};

#endif // QMLRENDER_H
