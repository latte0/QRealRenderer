#ifndef MOUSERENDERER_H
#define MOUSERENDERER_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>


#include <QOffscreenSurface>
#include <QOpenGLBuffer>
#include <QMutex>

#include "renderwindow.h"
#include "actor.h"
#include "handinfo.h"
#include "windowrenderer.h"
#include "eigenutil.h"

enum MouseMode : short{
  Sphere,
  OnWindow
};

class RenderWindow;

class MouseRenderer : public WindowRenderer
{
public:
    MouseRenderer();
    virtual ~MouseRenderer();

    virtual void bindTex();
    virtual void inittex(std::shared_ptr<QOpenGLContext>& share);
    virtual void collide(Eigen::Vector3f top);
    virtual void update();

    void setWindowPos(float x, float y);
    void setWindowPos(Eigen::Vector2f pos);

    void setAttendant(WindowRenderer *attendant);

    Eigen::Vector2f getWindowPos();

    QOpenGLTexture *m_mousetex;

    MouseMode mode = MouseMode::OnWindow;


private:
   Eigen::Vector2f m_windowpos; // this arrange of positions are 0 ~ 1
   WindowRenderer *m_attendant;

};

#endif // MOUSERENDERER_H
