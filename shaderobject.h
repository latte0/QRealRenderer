#ifndef SHADEROBJECT_H
#define SHADEROBJECT_H

#include <iostream>
#include <string>

#include <GL/gl.h>
#include <GL/glut.h>


#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLFunctions_4_3_Compatibility>

#include <QOpenGLShaderProgram>



class ShaderObject : protected QOpenGLFunctions_4_3_Core

{

public:
    ShaderObject();
    ~ShaderObject();

    int GenProgram(const std::string vertexShader, const std::string fragmentShader, const std::string Attribs[], int AttribsArraySize, const std::string Uniforms[], int UniformsArraySize, const std::string Textures[], int TextureArraySize);
    int GenProgram(const QString vertexShader, const QString fragmentShader);
    int GenLocation(const std::string Attribs[], int AttribsArraySize, const std::string Uniforms[], int UniformsArraySize, const std::string Textures[], int TextureArraySize);
    int GenCSProgram(const std::string computeShader, const std::string Uniforms[], int UniformsArraySize , const std::string Textures[], int TextureArraySize);

    void Release();
    int	auiLocation[32];
    int pid;
    int	Initialized;
    int	hashA;
    int hashB;
};

#endif // SHADEROBJECT_H
