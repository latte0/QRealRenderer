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
    virtual void init(std::shared_ptr<QOpenGLContext>& share, const std::string &filename);
    virtual void render(std::shared_ptr<QOpenGLContext>& share, UniformVs uniformvs);

    handFbxLoader m_handfbxLoader;


};

#endif // HANDFBXRENDERER_H

//if(handinfo->m_righthand) drawFunc(m_meshlist);
