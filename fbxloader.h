#ifndef FBXLOADER_H_
#define FBXLOADER_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <fbxsdk.h>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include "eigenutil.h"


using namespace Eigen;
using namespace QRRUtil;

struct ModelBoneWeight
{
    uint8_t boneIndex[4];
    Vector4f boneWeight;
};

struct ModelVertex
{
    Vector3f position;
    Vector3f normal;
    Vector2f uv0;
    uint8_t boneIndex[4];
    Vector4f boneWeight;

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

    Matrix4f invMeshBaseposeMatrix;
    std::vector<std::string> boneNodeNameList;
    std::vector<Matrix4f> invBoneBaseposeMatrixList;
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



class fbxLoader
{
public:

    fbxLoader();

    bool Initialize(const char* filepath);
    void Finalize();
    std::vector<int> GetIndexList(FbxMesh* mesh);

    bool LoadAnimation(const char* filepath);

    int GetMeshCount() const
    {
        return (int)this->meshList.size();
    }

    const ModelMesh& GetMesh(int index) const
    {
        return this->meshList[index];
    }

    int GetMaterialCount() const
    {
        return (int)this->materialList.size();
    }

    const int GetMaterialId(const std::string& materialName) const
    {
        return this->materialIdDictionary.at(materialName);
    }

    const ModelMaterial& GetMaterial(int materialId) const
    {
        return this->materialList[materialId];
    }

    float GetAnimationStartFrame() const
    {
        return this->animationStartFrame;
    }

    float GetAnimationEndFrame() const
    {
        return this->animationEndFrame;
    }

    void GetMeshMatrix(float frame, int meshId, Matrix4f& out_matrix) const;
    void GetBoneMatrix(float frame, int meshId, Matrix4f* out_matrixList, int matrixCount);


    FbxSkeleton* getSkeleton(std::string boneNodeName);

private:

  std::vector<Vector3f> GetNormalList(FbxMesh* mesh, const std::vector<int>& indexList);
  std::vector<Vector2f> GetUVList(FbxMesh* mesh, const std::vector<int>& indexList, int uvNo);
  std::vector<Vector3f> GetPositionList(FbxMesh* mesh, const std::vector<int>& indexList);

  void GetWeight(FbxMesh* mesh, const std::vector<int>& indexList, std::vector<ModelBoneWeight>& boneWeightList, std::vector<std::string>& boneNodeNameList, std::vector<Matrix4f>& invBaseposeMatrixList);

  ModelMesh ParseMesh(FbxMesh* mesh);
  ModelMaterial ParseMaterial(FbxSurfaceMaterial* material);

  FbxManager* sdkManager;
  FbxScene* fbxScene;

  std::vector<ModelMesh> meshList;
  std::vector<ModelMaterial> materialList;
  std::map<std::string, int> materialIdDictionary;
  std::map<std::string, int> nodeIdDictionary;

  FbxScene* fbxSceneAnimation;
  std::map<std::string, int> nodeIdDictionaryAnimation;
  float animationStartFrame;
  float animationEndFrame;
};

#endif

