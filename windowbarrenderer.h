#ifndef WINDOWBAR_H
#define WINDOWBAR_H

#include "windowrenderer.h"

class WindowBarRenderer : public WindowRenderer
{
public:
    WindowBarRenderer(QWindow* surface);
};

#endif // WINDOWBAR_H
