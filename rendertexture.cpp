#include "rendertexture.h"
#include <stdio.h>
#include <stdlib.h>

#include <QDebug>

#ifndef GL_RGBA16F
#define GL_RGBA16F GL_RGBA16F_ARB
#endif

bool RenderTexture::ms_useFiltering = true;

RenderTexture::RenderTexture():
    m_width(0U),
    m_height(0U),
    m_fboId(0),
    m_texId(0),
    m_pboId(0),
    m_depthRBId(0),
    m_colorRBId(0)
{
}

RenderTexture::~RenderTexture() {
    if (m_texId) {
        glDeleteTextures(1, &m_texId);
        m_texId = 0;
    }
    if (m_colorRBId) {
        glDeleteRenderbuffers(1, &m_colorRBId);
        m_colorRBId = 0;
    }
    if (m_depthRBId) {
        glDeleteRenderbuffers(1, &m_depthRBId);
        m_depthRBId = 0;
    }
    if (m_fboId) {
        glDeleteFramebuffers(1, &m_fboId);
        m_fboId = 0;
    }
    if (m_pboId) {
        glDeleteBuffers(1, &m_pboId);
    }
}

bool RenderTexture::Init(int width, int height, ColorType color, DepthType depth)
{

    qDebug() << "rendertexture init";

    GLenum err;
    m_width = width;
    m_height = height;
    glGenFramebuffers(1, &m_fboId);
    qDebug() << "gen";
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
    err = glGetError();

    qDebug() << "before fb init";

    if (color >= HDR) {
        // Create the HDR render target
        switch (color) {
        case RGB16F: m_format = GL_RGB;  m_type = GL_HALF_FLOAT; break;
        default:
        case RGBA16F: m_format = GL_RGBA; m_type = GL_HALF_FLOAT; break;
        }
        glGenTextures(1, &m_texId);
        glBindTexture(GL_TEXTURE_2D, m_texId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ms_useFiltering ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ms_useFiltering ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //note that internalformat and format have both to be GL_RGB or GL_RGBA for creating floating point texture.
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, width, height);
        err = glGetError();
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texId, 0);
        err = glGetError();

    } else {
        // Create the LDR render target
        m_type = GL_UNSIGNED_BYTE;
        glGenTextures(1, &m_texId);
        glBindTexture(GL_TEXTURE_2D, m_texId);
        switch (color) {
        case RGBA8888: m_format = GL_RGBA; break;
        default:
        case RGB888: m_format = GL_RGB; break;
        }
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexStorage2D(GL_TEXTURE_2D, 1, m_format, width, height);

        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texId, 0);
    }
    // Create depth buffer
    if (depth != NoDepth) {
        glGenRenderbuffers(1, &m_depthRBId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthRBId);
        err = glGetError();
        unsigned int depth_format = 0;
        switch (depth) {
        case Depth24: depth_format = GL_DEPTH_COMPONENT24; break;
        default:
        case Depth16: depth_format = GL_DEPTH_COMPONENT16; break;
        }
        glRenderbufferStorage(GL_RENDERBUFFER, depth_format, width, height);
        err = glGetError();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRBId);
        err = glGetError();
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    // Finalize
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    return true;
}

void RenderTexture::ActivateFB()
{
    //first save old FB, and then activate new FB
    //glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_prevFbo);
    //glGetIntegerv(GL_VIEWPORT, m_prevViewport);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
    glViewport(0, 0, m_width, m_height);
}

