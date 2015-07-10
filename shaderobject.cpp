#include "shaderobject.h"

ShaderObject::ShaderObject():
    Initialized{0},
    pid {0}
{

}

ShaderObject::~ShaderObject()
{

}

int ShaderObject::GenProgram(const std::string vertexShader, const std::string fragmentShader, const std::string Attribs[], int AttribsArraySize, const std::string Uniforms[], int UniformsArraySize, const std::string Textures[], int TextureArraySize)
{
    if (Initialized) return -1;

        int i;

        pid = glCreateProgram();

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);


        auto vs_char = vertexShader.data();
        auto fs_char = fragmentShader.data();
        glShaderSource(vs, (GLsizei)1, &vs_char, 0);
        glShaderSource(fs, 1,&fs_char, 0);

        glCompileShader(vs);

        GLint compiled = 0;
        glGetShaderiv(vs, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = new char[infoLen];
                if (buf) {
                    glGetShaderInfoLog(vs, infoLen, NULL, buf);
                    //LOGI("Shader log:\n%s\n", buf);
                    delete[] buf;
                }
            }
            return 0;
        }
        glAttachShader(pid, vs);
        glDeleteShader(vs);

        glCompileShader(fs);
        glGetShaderiv(fs, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = new char[infoLen];
                if (buf) {
                    glGetShaderInfoLog(fs, infoLen, NULL, buf);
                    //LOGI("Shader log:\n%s\n", buf);
                    delete[] buf;
                }
            }
            return 0;
        }
        glAttachShader(pid, fs);
        glDeleteShader(fs);


        glLinkProgram(pid);

        // check if program linked
        GLint success = 0;
        glGetProgramiv(pid, GL_LINK_STATUS, &success);

        if (!success)
        {
            GLint bufLength = 0;
            glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = new char[bufLength];
                if (buf) {
                    glGetProgramInfoLog(pid, bufLength, NULL, buf);
                    delete [] buf;
                }
            }
            glDeleteProgram(pid);
            pid = 0;
        }

        if (!pid) return -1;

        glUseProgram(pid);


        //bind the vertex attribute from string Attribs[i] to location i
        for (i = 0; i < AttribsArraySize; ++i)  glBindAttribLocation(pid, i, Attribs[i].data());


        //get location of uniform from string Uniforms[i]
        for (i=0; i<UniformsArraySize; i++)  auiLocation[i] = glGetUniformLocation(pid, Uniforms[i].data());


        int sid;
        //set the sampler2D named by string Textures[i] to texture i
        for (i=0; i<TextureArraySize; i++) {
            sid = glGetUniformLocation(pid, Textures[i].data());
            glUniform1i(sid, i);
        }

        Initialized = 1;
        return 0;
}
int ShaderObject::GenProgram(const QString vertexShader, const QString fragmentShader){
    auto prog = new QOpenGLShaderProgram();
    prog->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader);
    prog->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader);
    prog->link();
    if (prog) {
        pid = prog->programId();
        delete prog;
    }
    else pid =-1;

    return pid;

}
int ShaderObject::GenLocation(const std::string Attribs[], int AttribsArraySize, const std::string Uniforms[], int UniformsArraySize, const std::string Textures[], int TextureArraySize)
{
    int i;

    glUseProgram(pid);

    for (i = 0; i < AttribsArraySize; ++i) {
        glBindAttribLocation(pid, i, Attribs[i].data());
    }

    for (i=0; i<UniformsArraySize; i++) {
        auiLocation[i] = glGetUniformLocation(pid, Uniforms[i].data());
    }

    for (i=0; i<TextureArraySize; i++) {
        glUniform1i(glGetUniformLocation(pid, Textures[i].data()), i);
    }
    return 0;
}
int ShaderObject::GenCSProgram(const std::string computeShader, const std::string Uniforms[], int UniformsArraySize , const std::string Textures[], int TextureArraySize)
{
    int i;
        GLuint program = glCreateProgram();

        GLuint cs = glCreateShader(GL_COMPUTE_SHADER);

        auto cs_ch = computeShader.data();
        glShaderSource(cs, 1, &(cs_ch), 0);
        glCompileShader(cs);
        GLint compiled = 0;
        glGetShaderiv(cs, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(cs, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = new char[infoLen];
                if (buf) {
                    glGetShaderInfoLog(cs, infoLen, NULL, buf);
                    //LOGI("Shader log:\n%s\n", buf);
                    delete[] buf;
                }
            }
            return 0;
        }

        glAttachShader(program, cs);

        glDeleteShader(cs);

        glLinkProgram(program);

        GLint success = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success)
        {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = new char[bufLength];
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    //LOGI("Could not link program:\n%s\n", buf);
                    delete [] buf;
                }
            }
            glDeleteProgram(program);
            program = 0;
        }

        for (i=0; i<UniformsArraySize; i++) {
            auiLocation[i] = glGetUniformLocation(program, Uniforms[i].data());
        }
        int sid;

        for (i=0; i<TextureArraySize; i++) {
            sid = glGetUniformLocation(program, Textures[i].data());
            glUniform1i(sid, i);
        }

        pid = program;
        return pid;
}

void ShaderObject::Release(){}
