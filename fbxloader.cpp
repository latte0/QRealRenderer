#include <fbxsdk.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <QDebug>

#include <stdio.h>

#include "fbxloader.h"
#include "eigenutil.h"

//fbx no reader dayo
//dependent eigen libraryp

fbxLoader::fbxLoader(){
}

fbxLoader::~fbxLoader(){
}
std::vector<int> fbxLoader::GetIndexList(FbxMesh* mesh)
{
  // to triangel
  auto polygonCount = mesh->GetPolygonCount();

  std::vector<int> indexList;
  indexList.reserve(polygonCount * 3);

  for (int i = 0; i < polygonCount; ++i)
  {
    indexList.push_back(mesh->GetPolygonVertex(i, 0));
    indexList.push_back(mesh->GetPolygonVertex(i, 1));
    indexList.push_back(mesh->GetPolygonVertex(i, 2));
  }

  return indexList;
}

std::vector<Eigen::Vector3f> fbxLoader::GetPositionList(FbxMesh* mesh, const std::vector<int>& indexList)
{
  // controlpoint
  std::vector<Eigen::Vector3f> positionList;
  positionList.reserve(indexList.size());

  for (auto index : indexList)
  {
    auto controlPoint = mesh->GetControlPointAt(index);

    positionList.push_back(QRRUtil::EigenVector3fMake(controlPoint[0], controlPoint[1], controlPoint[2]));

    Q_ASSERT(controlPoint[3] == 0.0);
  }

  return positionList;
}

std::vector<Eigen::Vector3f> fbxLoader::GetNormalList(FbxMesh* mesh, const std::vector<int>& indexList)
{
  auto elementCount = mesh->GetElementNormalCount();

  //  normarl vector is 1
  Q_ASSERT(elementCount == 1);

  auto element = mesh->GetElementNormal();
  auto mappingMode = element->GetMappingMode();
  auto referenceMode = element->GetReferenceMode();
  const auto& indexArray = element->GetIndexArray();
  const auto& directArray = element->GetDirectArray();


  Q_ASSERT((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

  std::vector<Eigen::Vector3f> normalList;
  normalList.reserve(indexList.size());

  if (mappingMode == FbxGeometryElement::eByControlPoint)
  {
    // controll point mapping
    for (auto index : indexList)
    {
      auto normalIndex = (referenceMode == FbxGeometryElement::eDirect)
      ? index
      : indexArray.GetAt(index);
      auto normal = directArray.GetAt(normalIndex);
      normalList.push_back(QRRUtil::EigenVector3fMake(normal[0], normal[1], normal[2]));


      //Q_ASSERT(normal[3] == 1.0);
    }
  }
  else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
  {

    auto indexByPolygonVertex = 0;
    auto polygonCount = mesh->GetPolygonCount();
    for (int i = 0; i < polygonCount; ++i)
    {
      auto polygonSize = mesh->GetPolygonSize(i);

      for (int j = 0; j < polygonSize; ++j)
      {
        auto normalIndex = (referenceMode == FbxGeometryElement::eDirect)
        ? indexByPolygonVertex
        : indexArray.GetAt(indexByPolygonVertex);
        auto normal = directArray.GetAt(normalIndex);

        normalList.push_back(QRRUtil::EigenVector3fMake(normal[0], normal[1], normal[2]));


        //Q_ASSERT(normal[3] == 1.0);

        ++indexByPolygonVertex;
      }

    }
  }
  else
  {

    Q_ASSERT(false);
  }

  return normalList;
}

std::vector<Eigen::Vector2f> fbxLoader::GetUVList(FbxMesh* mesh, const std::vector<int>& indexList, int uvNo)
{
  std::vector<Eigen::Vector2f> uvList;

  auto elementCount = mesh->GetElementUVCount();
  if (uvNo + 1 > elementCount)
  {
    return uvList;
  }

  auto element = mesh->GetElementUV(uvNo);
  auto mappingMode = element->GetMappingMode();
  auto referenceMode = element->GetReferenceMode();
  const auto& indexArray = element->GetIndexArray();
  const auto& directArray = element->GetDirectArray();


  Q_ASSERT((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

  uvList.reserve(indexList.size());

  if (mappingMode == FbxGeometryElement::eByControlPoint)
  {

    for (auto index : indexList)
    {
      auto uvIndex = (referenceMode == FbxGeometryElement::eDirect)
      ? index
      : indexArray.GetAt(index);
      auto uv = directArray.GetAt(uvIndex);
      uvList.push_back(QRRUtil::EigenVector2fMake(uv[0], uv[1]));
    }
  }
  else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
  {

    auto indexByPolygonVertex = 0;
    auto polygonCount = mesh->GetPolygonCount();
    for (int i = 0; i < polygonCount; ++i)
    {
      auto polygonSize = mesh->GetPolygonSize(i);

      for (int j = 0; j < polygonSize; ++j)
      {
        auto uvIndex = (referenceMode == FbxGeometryElement::eDirect)
        ? indexByPolygonVertex
        : indexArray.GetAt(indexByPolygonVertex);
        auto uv = directArray.GetAt(uvIndex);

        uvList.push_back(QRRUtil::EigenVector2fMake(uv[0], uv[1]));

        ++indexByPolygonVertex;
      }

    }
  }
  else
  {
    Q_ASSERT(false);
  }

  return uvList;
}

void fbxLoader::GetWeight(FbxMesh* mesh, const std::vector<int>& indexList, std::vector<ModelBoneWeight>& boneWeightList, std::vector<std::string>& boneNodeNameList, std::vector<Eigen::Matrix4f>& invBaseposeMatrixList)
{
  auto skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
  if (skinCount == 0)
  {
    return;
  }
  Q_ASSERT(skinCount <= 1);

  auto controlPointsCount = mesh->GetControlPointsCount();
  using TmpWeight = std::pair<int, float>;
  std::vector<std::vector<TmpWeight>> tmpBoneWeightList(controlPointsCount);

  auto skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
  auto clusterCount = skin->GetClusterCount();

  for (int i = 0; i < clusterCount; ++i)
  {
    auto cluster = skin->GetCluster(i);

    Q_ASSERT(cluster->GetLinkMode() == FbxCluster::eNormalize);

    boneNodeNameList.push_back(cluster->GetLink()->GetName());

    auto indexCount = cluster->GetControlPointIndicesCount();
    auto indices = cluster->GetControlPointIndices();
    auto weights = cluster->GetControlPointWeights();

    for (int j = 0; j < indexCount; ++j)
    {
      auto controlPointIndex = indices[j];
      tmpBoneWeightList[controlPointIndex].push_back({i, weights[j]});
    }

    // create bsepose matrix ivert
    Eigen::Matrix4f invBaseposeMatrix;

    auto baseposeMatrix = cluster->GetLink()->EvaluateGlobalTransform().Inverse();
    auto baseposeMatrixPtr = (double*)baseposeMatrix;
    for (int j = 0; j < 16; ++j)
    {
      invBaseposeMatrix(j) = baseposeMatrixPtr[j];
    }

    invBaseposeMatrixList.push_back(invBaseposeMatrix);
  }


  std::vector<ModelBoneWeight> boneWeightListControlPoints;
  for (auto& tmpBoneWeight : tmpBoneWeightList)
  {
    // weight sort
    std::sort(tmpBoneWeight.begin(), tmpBoneWeight.end(),
              [](const TmpWeight& weightA, const TmpWeight& weightB){ return weightA.second > weightB.second; }
              //[](const TmpWeight& weightA, const TmpWeight& weightB){ return weightA.second < weightB.second; }
    );

    // ignore
    while (tmpBoneWeight.size() > 4)
    {
      tmpBoneWeight.pop_back();
    }

    // insert dummy
    while (tmpBoneWeight.size() < 4)
    {
      tmpBoneWeight.push_back({0, 0.0f});
    }

    ModelBoneWeight weight;
    float total = 0.0f;
    for (int i = 0; i < 4; ++i)
    {
      weight.boneIndex[i] = tmpBoneWeight[i].first;
      weight.boneWeight(i) = tmpBoneWeight[i].second;

      total += tmpBoneWeight[i].second;
    }

    // normalyze weight
    for (int i = 0; i < 4; ++i)
    {
      weight.boneWeight(i) /= total;
    }

    boneWeightListControlPoints.push_back(weight);
  }

  // deploy with index
  for (auto index : indexList)
  {
    boneWeightList.push_back(boneWeightListControlPoints[index]);
  }
}

bool fbxLoader::Initialize(const char* filepath)
{
  this->sdkManager = FbxManager::Create();

  auto ios = FbxIOSettings::Create(this->sdkManager, IOSROOT);
  this->sdkManager->SetIOSettings(ios);

  auto importer = FbxImporter::Create(this->sdkManager, "");

  if (!importer->Initialize(filepath, -1, this->sdkManager->GetIOSettings()))
  {
    return false;
  }

  this->fbxScene = FbxScene::Create(this->sdkManager, "originalScene");
  importer->Import(this->fbxScene);
  importer->Destroy();

  // to change allpolygon to triangle
  FbxGeometryConverter geometryConverter(this->sdkManager);
  geometryConverter.Triangulate(this->fbxScene, true);

  // registr nodename to get it with nodeid
  auto nodeCount = this->fbxScene->GetNodeCount();
  qDebug() <<"nodeCount: %d\n"<< nodeCount;
  for (int i = 0; i < nodeCount; ++i)
  {
    auto fbxNode = this->fbxScene->GetNode(i);
    this->nodeIdDictionary.insert({fbxNode->GetName(), i});
  }

  /*// analyze material
  auto materialCount = this->fbxScene->GetMaterialCount();
  this->materialList.reserve(materialCount);
  qDebug() << "materialCount: %d\n" << materialCount;
  for (int i = 0; i < materialCount; ++i)
  {
    auto fbxMaterial = this->fbxScene->GetMaterial(i);
    auto modelMaterial = this->ParseMaterial(fbxMaterial);
    this->materialList.push_back(modelMaterial);
    this->materialIdDictionary.insert({modelMaterial.materialName, i});
  }*/

  // analyze with in scene
  auto meshCount = this->fbxScene->GetMemberCount<FbxMesh>();
  this->meshList.reserve(meshCount);
  qDebug() << "meshCount: %d\n" << meshCount;
  for (int i = 0; i < meshCount; ++i)
  {
    auto fbxMesh = this->fbxScene->GetMember<FbxMesh>(i);
    this->meshList.push_back(this->ParseMesh(fbxMesh));
  }


  return true;
}



void fbxLoader::Finalize()
{
  this->sdkManager->Destroy();

  this->meshList.clear();
  this->materialList.clear();
  this->materialIdDictionary.clear();
  this->nodeIdDictionary.clear();

}

bool fbxLoader::LoadAnimation(const char* filepath)
{
  auto importer = FbxImporter::Create(this->sdkManager, "");

  if (!importer->Initialize(filepath, -1, this->sdkManager->GetIOSettings()))
  {
    return false;
  }

  this->fbxSceneAnimation = FbxScene::Create(this->sdkManager, "animationScene");
  importer->Import(this->fbxSceneAnimation);

  //get animation frame
  auto animStackCount = importer->GetAnimStackCount();
  //Q_ASSERT(animStackCount == 1);
  auto takeInfo = importer->GetTakeInfo(0);

  auto importOffset = takeInfo->mImportOffset;
  auto startTime = takeInfo->mLocalTimeSpan.GetStart();
  auto stopTime = takeInfo->mLocalTimeSpan.GetStop();

  this->animationStartFrame = (importOffset.Get() + startTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);
  this->animationEndFrame = (importOffset.Get() + stopTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);
  importer->Destroy();

  // to registr nodename to get it with nodeid
  auto nodeCount = this->fbxSceneAnimation->GetNodeCount();
  qDebug() << "animationNodeCount: %d\n" << nodeCount;
  for (int i = 0; i < nodeCount; ++i)
  {
    auto fbxNode = this->fbxSceneAnimation->GetNode(i);
    this->nodeIdDictionaryAnimation.insert({fbxNode->GetName(), i});
  }

  return true;
}

ModelMesh fbxLoader::ParseMesh(FbxMesh* mesh)
{
  Q_ASSERT(mesh != nullptr);

  auto node = mesh->GetNode();

  // material one
  //Q_ASSERT(node->GetMaterialCount() == 1);

  ModelMesh modelMesh;
  modelMesh.nodeName = node->GetName();
  modelMesh.materialName ="";//= node->GetMaterial(0)->GetName();

  qDebug() << ">> mesh: %s, %s\n" << modelMesh.nodeName.c_str() << modelMesh.materialName.c_str();

  //base pose inverse
  auto baseposeMatrix = node->EvaluateGlobalTransform().Inverse();

  auto baseposeMatrixPtr = (double*)baseposeMatrix;
  for (int i = 0; i < 16; ++i)
  {
    modelMesh.invMeshBaseposeMatrix(i) = baseposeMatrixPtr[i];
  }

  // get iondex
  auto indexList = GetIndexList(mesh);

  // get all vertex
  auto positionList = GetPositionList(mesh, indexList);
  auto normalList = GetNormalList(mesh, indexList);
  auto uv0List = GetUVList(mesh, indexList, 0);

  std::vector<ModelBoneWeight> boneWeightList;
  GetWeight(mesh, indexList, boneWeightList, modelMesh.boneNodeNameList, modelMesh.invBoneBaseposeMatrixList);

  // size check
  Q_ASSERT(indexList.size() == positionList.size());
  Q_ASSERT(indexList.size() == normalList.size());
  Q_ASSERT(indexList.size() == uv0List.size());
  Q_ASSERT((indexList.size() == boneWeightList.size()) || (boneWeightList.size() == 0));


  std::vector<ModelVertex> modelVertexList;
  modelVertexList.reserve(indexList.size());

  for (int i = 0; i < indexList.size(); ++i)
  {
    ModelVertex vertex;
    vertex.position = positionList[i];
    vertex.normal = normalList[i];
    if(uv0List.size() == 0) vertex.uv0 = QRRUtil::EigenVector2fMake(0.0f, 0.0f);
    else vertex.uv0= uv0List[i];

    if (boneWeightList.size() > 0)
    {
      for (int j = 0; j < 4; ++j)
      {
        vertex.boneIndex[j] = boneWeightList[i].boneIndex[j];
      }
      vertex.boneWeight = boneWeightList[i].boneWeight;
    }
    else
    {
      for (int j = 0; j < 4; ++j)
      {
        vertex.boneIndex[j] = 0;
      }
      vertex.boneWeight = QRRUtil::EigenVector4fMake(1, 0, 0, 0);
    }

  // qDebug() << "weight " <<  i <<  vertex.boneWeight.x <<  vertex.boneWeight.y <<  vertex.boneWeight.z << vertex.boneWeight.w;
    modelVertexList.push_back(vertex);
  }

  // can glDrawarray();

  // create index
  auto& modelVertexListOpt = modelMesh.vertexList;
  modelVertexListOpt.reserve(modelVertexList.size());

  auto& modelIndexList = modelMesh.indexList;
  modelIndexList.reserve(indexList.size());

  for (auto& vertex : modelVertexList)
  {
    // overlap?
    auto it = std::find(modelVertexListOpt.begin(), modelVertexListOpt.end(), vertex);
    if (it == modelVertexListOpt.end())
    {
      // not overlap
      modelIndexList.push_back(modelVertexListOpt.size());
      modelVertexListOpt.push_back(vertex);
    }
    else
    {
      // overlap
      auto index = std::distance(modelVertexListOpt.begin(), it);
      modelIndexList.push_back(index);
    }
  }

  // can glDrawelement()
  qDebug() << "Opt: %lu -> %lu\n" << modelVertexList.size() << modelVertexListOpt.size();

  return modelMesh;
}

ModelMaterial fbxLoader::ParseMaterial(FbxSurfaceMaterial* material)
{
  ModelMaterial modelMaterial;
  modelMaterial.materialName = material->GetName();

  qDebug() << ">> material: %s\n" << modelMaterial.materialName.c_str();

  // no filetr
    auto implementation = GetImplementation(material, FBXSDK_IMPLEMENTATION_CGFX);

    qDebug() << "get implementaito";
 // Q_ASSERT(implementation != nullptr);

  auto rootTable = implementation->GetRootTable();
  auto entryCount = rootTable->GetEntryCount();

  qDebug() << "getroottabkle";

  for (int i = 0; i < entryCount; ++i)
  {
    auto entry = rootTable->GetEntry(i);

    auto fbxProperty = material->FindPropertyHierarchical(entry.GetSource());
    if (!fbxProperty.IsValid())
    {
      fbxProperty = material->RootProperty.FindHierarchical(entry.GetSource());
    }

    auto textureCount = fbxProperty.GetSrcObjectCount<FbxTexture>();
    if (textureCount > 0)
    {
      std::string src = entry.GetSource();

      qDebug() << src.data();

      for (int j = 0; j < fbxProperty.GetSrcObjectCount<FbxFileTexture>(); ++j)
      {
        auto tex = fbxProperty.GetSrcObject<FbxFileTexture>(j);
        std::string texName = tex->GetFileName();
        texName = texName.substr(texName.find_last_of('/') + 1);

        if (src == "Maya|DiffuseTexture")
        {
          modelMaterial.diffuseTextureName = texName;
        }
        else if (src == "Maya|NormalTexture")
        {
          modelMaterial.normalTextureName = texName;
        }
        else if (src == "Maya|SpecularTexture")
        {
          modelMaterial.specularTextureName = texName;
        }
        else if (src == "Maya|FalloffTexture")
        {
          modelMaterial.falloffTextureName = texName;
        }
        else if (src == "Maya|ReflectionMapTexture")
        {
          modelMaterial.reflectionMapTextureName = texName;
        }
      }
    }
  }

  qDebug() << "diffuseTexture: %s\n" << modelMaterial.diffuseTextureName.c_str();
  qDebug() << "normalTexture: %s\n" << modelMaterial.normalTextureName.c_str() ;
  qDebug() << "specularTexture: %s\n" << modelMaterial.specularTextureName.c_str();
  qDebug() <<"falloffTexture: %s\n", modelMaterial.falloffTextureName.c_str();
  qDebug() <<"reflectionMapTexture: %s\n", modelMaterial.reflectionMapTextureName.c_str();

  return modelMaterial;
}

void fbxLoader::GetMeshMatrix(float frame, int meshId, Eigen::Matrix4f& out_matrix) const
{
  auto& modelMesh = this->meshList[meshId];

  auto it = this->nodeIdDictionaryAnimation.find(modelMesh.nodeName);

  if (it == this->nodeIdDictionaryAnimation.end())
  {
    out_matrix = Eigen::Matrix4f::Identity();
    return;
  }

  auto meshNodeId = it->second;
  auto meshNode = this->fbxSceneAnimation->GetNode(meshNodeId);

  FbxTime time;
  time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * frame);

  auto& meshMatrix = meshNode->EvaluateGlobalTransform(time);

  auto meshMatrixPtr = (double*)meshMatrix;

  for (int i = 0; i < 16; ++i)
  {
    out_matrix(i) = (float)meshMatrixPtr[i];
  }

  out_matrix = out_matrix * modelMesh.invMeshBaseposeMatrix;
}

void fbxLoader::GetBoneMatrix(float frame, int meshId, Eigen::Matrix4f* out_matrixList, int matrixCount)
{
  auto& modelMesh = this->meshList[meshId];



  if (modelMesh.boneNodeNameList.size() == 0)
  {
    out_matrixList[0] = Eigen::Matrix4f::Identity();
    return;
  }

  Q_ASSERT(modelMesh.boneNodeNameList.size() <= matrixCount);

  FbxTime time;
  time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * frame);

  for (int i = 0; i < modelMesh.boneNodeNameList.size(); ++i)
  {
    auto& boneNodeName = modelMesh.boneNodeNameList[i];
    auto boneNodeId = this->nodeIdDictionaryAnimation.at(boneNodeName);
    auto boneNode = this->fbxSceneAnimation->GetNode(boneNodeId);

    auto& boneMatrix = boneNode->EvaluateGlobalTransform(time);
    auto& out_matrix = out_matrixList[i];

    auto boneMatrixPtr = (double*)boneMatrix;
    for (int j = 0; j < 16; ++j)
    {
      out_matrix(j) = (float)boneMatrixPtr[j];
    }


    Eigen::Matrix4f trans = Eigen::Matrix4f::Identity();



    out_matrix = out_matrix * modelMesh.invBoneBaseposeMatrixList[i];

    }


}




FbxSkeleton* fbxLoader::getSkeleton(std::string boneNodeName)
{
    auto boneNodeId = this->nodeIdDictionaryAnimation.at(boneNodeName);
    auto boneNode = this->fbxSceneAnimation->GetNode(boneNodeId);

    FbxNodeAttribute* boneAttribute = boneNode->GetNodeAttribute();
    return static_cast<FbxSkeleton*>(boneAttribute);
}
