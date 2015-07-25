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
#include "fbxstruct.h"



class fbxLoader
{
    friend class FbxRenderer;
public:

    fbxLoader();
    ~fbxLoader();

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

    void GetMeshMatrix(float frame, int meshId, Eigen::Matrix4f& out_matrix) const;
    void GetBoneMatrix(float frame, int meshId, Eigen::Matrix4f* out_matrixList, int matrixCount);


    FbxSkeleton* getSkeleton(std::string boneNodeName);

public:

  std::vector<Eigen::Vector3f> GetNormalList(FbxMesh* mesh, const std::vector<int>& indexList);
  std::vector<Eigen::Vector2f> GetUVList(FbxMesh* mesh, const std::vector<int>& indexList, int uvNo);
  std::vector<Eigen::Vector3f> GetPositionList(FbxMesh* mesh, const std::vector<int>& indexList);

  void GetWeight(FbxMesh* mesh, const std::vector<int>& indexList, std::vector<ModelBoneWeight>& boneWeightList, std::vector<std::string>& boneNodeNameList, std::vector<Eigen::Matrix4f>& invBaseposeMatrixList);

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

