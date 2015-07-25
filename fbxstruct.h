#ifndef FBXSTRUCT
#define FBXSTRUCT

#include "glutil.h"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include "fbxloader.h"


#define MAX_BONE_COUNT 128

#define BUFFER_OFFSET(i) ((char *)NULL + (i))





struct ModelBoneWeight
{
    uint8_t boneIndex[4];
    Eigen::Vector4f boneWeight;
};

struct ModelVertex
{
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
    Eigen::Vector2f uv0;
    uint8_t boneIndex[4];
    Eigen::Vector4f boneWeight;

    bool operator == (const ModelVertex& v) const
    {
        return std::memcmp(this, &v, sizeof(ModelVertex)) == 0;
    }
};

struct ModelMesh
{
    std::string nodeName;
    std::string materialName;

    std::vector<ModelVertex> vertexList;
    std::vector<uint16_t> indexList;

    Eigen::Matrix4f invMeshBaseposeMatrix;
    std::vector<std::string> boneNodeNameList;
    std::vector<Eigen::Matrix4f> invBoneBaseposeMatrixList;
};


struct ModelMaterial
{
    std::string materialName;

    std::string diffuseTextureName;
    std::string normalTextureName;
    std::string specularTextureName;
    std::string falloffTextureName;
    std::string reflectionMapTextureName;
};




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

