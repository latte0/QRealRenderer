#include "glutil.h"

#include "external/external_opengl/glm/glm.hpp"
#include "external/external_opengl/glm/gtc/type_ptr.hpp"


unsigned char* QRR::GLUtil::make_dummy_texture (int* width_, int* height_)
{
        int height = 256;
        int width  = 256;
        unsigned char* pixels = (unsigned char*)malloc (width*height*4);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                pixels[y*(width*4) + x*4 +0] = x % 256;
                pixels[y*(width*4) + x*4 +1] = y % 256;
                pixels[y*(width*4) + x*4 +2] = 0;
                pixels[y*(width*4) + x*4 +3] = 255;
            }
        }
        *width_  = width;
        *height_ = height;
        return pixels;
}
