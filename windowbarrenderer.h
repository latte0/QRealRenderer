#ifndef WINDOWBAR_H
#define WINDOWBAR_H

#include "windowrenderer.h"

class WindowBarRenderer : public WindowRenderer
{
public:
    WindowBarRenderer( std::shared_ptr<QOpenGLContext>& share);
};

#endif // WINDOWBAR_H
