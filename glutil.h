#ifndef GLUTIL_H
#define GLUTIL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>


#include "external/external_opengl/FreeImage/FreeImage.h"
#include "external/cppitertools/itertools.hpp"
#include "external/external_opengl/glm/glm.hpp"

namespace QRR{
    namespace GLUtil{
        unsigned char* make_dummy_texture (int* width_, int* height_);
    }
}

#endif // GLUTIL_H
