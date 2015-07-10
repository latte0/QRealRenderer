#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H


#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLFunctions_4_3_Compatibility>

class RenderTexture : protected QOpenGLFunctions_4_3_Core
{
public:
    enum DepthType {
        Depth24,
        Depth16,
        NoDepth
    };
    enum ColorType {
        RGB888,
        RGBA8888,
        HDR = 0xFFFF,
        RG16F,
        RGB16F,
        RGBA16F,
    };

    RenderTexture();
    ~RenderTexture();
    bool Init(int width, int height, ColorType color, DepthType depth);

    void ActivateFB();
    //void DeactivateFB();
    int GetWidth() {
        return m_width;
    }
    int GetHeight() {
        return m_height;
    }
    void Bind() { glBindTexture(GL_TEXTURE_2D, m_texId); }
    void Release() { glBindTexture(GL_TEXTURE_2D, 0); }
    unsigned int GetTexId() { return m_texId; }
    static bool ms_useFiltering;

private:
    int m_width;
    int m_height;
    int m_pboSize;
    unsigned int m_format;
    unsigned int m_type;
    unsigned int m_fboId;
    unsigned int m_texId;
    unsigned int m_pboId;
    unsigned int m_depthRBId;
    unsigned int m_colorRBId;
    int m_prevFbo;
    int m_prevViewport[4];
};
#endif // RENDERTEXTURE_H
