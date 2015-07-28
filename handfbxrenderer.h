#ifndef HANDFBXRENDERER_H
#define HANDFBXRENDERER_H

#include "fbxrenderer.h"
#include "handFbxLoader.h"

class handFbxRenderer : public FbxRenderer
{
public:
    handFbxRenderer();
    ~handFbxRenderer();

    virtual void update( UniformVs uniformvs);
    virtual void init(QOpenGLContext *share, const std::string &filename);
    virtual void render(QOpenGLContext *share, UniformVs uniformvs);

    handFbxLoader m_handfbxLoader;

};

#endif // HANDFBXRENDERER_H

//if(handinfo->m_righthand) drawFunc(m_meshlist);
