#ifndef FBXSTRUCT
#define FBXSTRUCT

#include "glutil.h"

#include <Eigen/Core>
#include <Eigen/Geometry>


#define MAX_BONE_COUNT 128

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


enum
{
    UNIFORM_VS,
    UNIFORM_DIFFUSE_TEXTURE,
    UNIFORM_FALLOFF_TEXTURE,
    UNIFORM_RIMLIGHT_TEXTURE,
    UNIFORM_SPECULAR_TEXTURE,
    NUM_UNIFORMS
};

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
    Eigen::Matrix4f boneMatrixList[MAX_BONE_COUNT];
};

#endif // FBXSTRUCT

