#ifndef FBXDRAWER_H
#define FBXDRAWER_H

#include "scenerendermodel.h"
#include "fbxsdk.h"
#include "fbxloader.h"
#include "camera.h"
#include "shaderobject.h"
//#include "fbxLoader.h"
//#include "objLoader.h"


#include <GL/gl.h>
#include "external/external_opengl/FreeImage/FreeImage.h"


#include <Eigen/Core>
#include <Eigen/Geometry>

#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


class fbxDrawer : public SceneRenderModel
{
public:
    fbxDrawer();
    ~fbxDrawer();

    constexpr static int m_bonemaxcount = 128;

    GLuint loadTexture (const std::string &filename);

    enum
    {
        UNIFORM_VS,
        UNIFORM_DIFFUSE_TEXTURE,
        UNIFORM_FALLOFF_TEXTURE,
        UNIFORM_RIMLIGHT_TEXTURE,
        UNIFORM_SPECULAR_TEXTURE,
        NUM_UNIFORMS
    };
    GLint uniformsSkin[NUM_UNIFORMS];
    GLint uniformsCloth[NUM_UNIFORMS];

    enum
    {
        ATTRIB_VERTEX,
        ATTRIB_NORMAL,
        ATTRIB_TEXCOORD0,
        ATTRIB_BONE_INDEX,
        ATTRIB_BONE_WEIGHT,
        NUM_ATTRIBUTES
    };

    struct AppMaterial
    {
        GLuint diffuseTexture;
        GLuint falloffTexture;
        GLuint specularTexture;
        GLuint rimlightTexture;
        GLuint repeatSampler;
        GLuint clampSampler;

        bool diffuseHasAlpha;

        const ModelMaterial* modelMaterial;
    };

    struct AppMesh
    {
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint indexBuffer;

        const AppMaterial* material;
        const ModelMesh* modelMesh;

        int modelMeshId;
    };

    struct UniformVs
    {
        Eigen::Matrix4f modelViewMatrix;
        Eigen::Matrix4f projectionMatrix;
        Eigen::Matrix4f normalMatrix;

        Eigen::Vector4f lightDirection;
        Eigen::Vector4f cameraPosition;

        Eigen::Matrix4f meshMatrix;
        Eigen::Matrix4f boneMatrixList[m_bonemaxcount];
    };

    void Initialize(ModelUpdate &&updatedata);
    void ReloadShader(std::string sh_filename);

   // HandInfo m_handinfo;

    GLuint m_uniformBufferVs;
    UniformVs m_uniformVs;


    fbxLoader m_fbxLoader;

    std::map<std::string, GLuint> m_textureDictionary;
    std::vector<AppMaterial> m_materialList;
    std::vector<AppMesh> m_meshlist;

    Eigen::Matrix4f m_eproj;
    Eigen::Matrix4f m_ecamera;
    Eigen::Matrix4f m_eworld;

    GLuint m_pid;


    virtual void render() override;
    virtual void update(ModelUpdate&&) override;
    virtual void set() override;

private:
    ShaderObject m_shaderobj;
    VertexBufferObject m_vertbufobj;


};

#endif // FBXDRAWER_H
