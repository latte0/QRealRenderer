#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H


#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Compatibility>

class VertexBufferObject :  protected QOpenGLFunctions_3_3_Core
{
public:
    VertexBufferObject();
    ~VertexBufferObject();
    void GenVertexData(void *data, unsigned int length, bool stream=false);
    void GenIndexData(void *data, unsigned int length, bool stream=false);
    void AddAttribute(int attribute, int count, int type, bool noramlize, int stride, int offset);
    void Draw(unsigned int type, unsigned int count, unsigned int data_type, int offset);
    unsigned int GetVBO() { return m_vboId; }
    unsigned int GetIBO() { return m_iboId; }

private:
    unsigned int m_vboId;
    unsigned int m_iboId;
};

#endif // VERTEXBUFFEROBJECT_H
