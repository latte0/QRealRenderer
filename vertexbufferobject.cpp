#include "vertexbufferobject.h"


VertexBufferObject::VertexBufferObject()
    : m_vboId(0),
      m_iboId(0)
{
}

VertexBufferObject::~VertexBufferObject()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &m_vboId);
  glDeleteBuffers(1, &m_iboId);
}

void VertexBufferObject::GenVertexData(void *data, unsigned int length, bool stream)
{
    glGenBuffers(1, &m_vboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    glBufferData(GL_ARRAY_BUFFER, length, data, stream?GL_STREAM_DRAW:GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::GenIndexData(void *data, unsigned int length, bool stream)
{
    glGenBuffers(1, &m_iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, length, data, stream?GL_STREAM_DRAW:GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexBufferObject::AddAttribute(int attribute, int count, int type, bool normalize, int stride, int offset)
{
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(attribute, count, type, normalize, stride, (GLvoid*)offset);
}

void VertexBufferObject::Draw(unsigned int type, unsigned int count, unsigned int data_type, int offset) {
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
    glDrawElements(type, count, data_type, (GLvoid*)offset);
}
