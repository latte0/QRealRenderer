#include <fbxsdk.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <QDebug>

#include <stdio.h>

#include "handFbxLoader.h"
#include "eigenutil.h"


using namespace Eigen;
using namespace QRRUtil;

//fbx no reader dayo
//dependent eigen libraryp

handFbxLoader::handFbxLoader(){
}

handFbxLoader::~handFbxLoader(){

}


void handFbxLoader::GetBoneMatrix(float frame, int meshId, Matrix4f* out_matrixList, int matrixCount, HandInfo* handInfo)
{
  auto& modelMesh = this->meshList[meshId];



  if (modelMesh.boneNodeNameList.size() == 0)
  {
    out_matrixList[0] = Matrix4f::Identity();
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


   // std::cout << boneNodeName <<"\n"<<boneAttribute->LimbLength  << "\n" << boneAttribute->GetAttributeType() << std::endl;

    auto& boneMatrix = boneNode->EvaluateGlobalTransform(time);
    auto& out_matrix = out_matrixList[i];

    auto boneMatrixPtr = (double*)boneMatrix;
    for (int j = 0; j < 16; ++j)
    {
      out_matrix(j) = (float)boneMatrixPtr[j];
    }


  //  std::cout << i << "\n" << out_matrix << std::endl ;

   // std::cout << boneNodeName<< "\n" << modelMesh.invBoneBaseposeMatrixList[i] << std::endl;
  //  std::cout << boneNodeName << std::endl;

    Matrix4f trans = Matrix4f::Identity();
    if(handInfo->m_righthand){


        if ( boneNodeName == "Bone" ) {
          //  trans = handInfo->m_handGlobalNorm;
           // trans = QRRUtil::MakeTransform(handInfo->m_handGlobalPos) * trans;
               if(boneNode->GetParent()->GetParent() != NULL){
                   //std::cout << "tashika ni tsuzuki ga aru !!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                FbxNodeAttribute* boneAttributea = boneNode->GetParent()->GetParent()->GetNodeAttribute();

             //   qDebug() << "a";
           //     std::cout << boneAttributea->GetAttributeType() << std::endl;
            }
        //    else std::cout << "tsuzuki nashi " << std::endl;
        }
        else if ( boneNodeName == "Bone_001" ) {
            trans = handInfo->m_fingerdata[1][0].position_mat * handInfo->m_fingerdata[1][0].direction_mat;

        }
        else if ( boneNodeName == "Bone_002" ) {
         //   Eigen::Vector3f v = getSkeleton("Bone_001")->LimbLength * handInfo->m_fingerdata[1][0].direction;

            trans = handInfo->m_fingerdata[1][1].position_mat * handInfo->m_fingerdata[1][1].direction_mat;
        }
        else if ( boneNodeName == "Bone_003" ) {
           // Eigen::Vector3f v = getSkeleton("Bone_001")->LimbLength * handInfo->m_fingerdata[1][0].direction +   getSkeleton("Bone_002")->LimbLength * handInfo->m_fingerdata[1][1].direction;

            trans = handInfo->m_fingerdata[1][2].position_mat * handInfo->m_fingerdata[1][2].direction_mat;
        }
        else if ( boneNodeName == "Bone_004" ) {
           // Eigen::Vector3f v = getSkeleton("Bone_001")->LimbLength * handInfo->m_fingerdata[1][0].direction +   getSkeleton("Bone_002")->LimbLength * handInfo->m_fingerdata[1][1].direction +  getSkeleton("Bone_003")->LimbLength * handInfo->m_fingerdata[1][2].direction;

            trans = handInfo->m_fingerdata[1][3].position_mat * handInfo->m_fingerdata[1][3].direction_mat;
        }

        else if ( boneNodeName == "Bone_005" ) {

            trans = handInfo->m_fingerdata[2][0].direction_mat * trans;
            trans = handInfo->m_fingerdata[2][0].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_006" ) {
            trans = handInfo->m_fingerdata[2][1].direction_mat * trans;
            trans = handInfo->m_fingerdata[2][1].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_007" ) {
            trans = handInfo->m_fingerdata[2][2].direction_mat * trans;
            trans = handInfo->m_fingerdata[2][2].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_008" ) {
            trans = handInfo->m_fingerdata[2][3].direction_mat * trans;
            trans = handInfo->m_fingerdata[2][3].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_009" ) {
            trans = handInfo->m_fingerdata[3][0].direction_mat * trans;
            trans = handInfo->m_fingerdata[3][0].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_010" ) {
            trans = handInfo->m_fingerdata[3][1].direction_mat * trans;
            trans = handInfo->m_fingerdata[3][1].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_011" ) {
            trans = handInfo->m_fingerdata[3][2].direction_mat * trans;
            trans = handInfo->m_fingerdata[3][2].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_012" ) {
            trans = handInfo->m_fingerdata[3][3].direction_mat * trans;
            trans = handInfo->m_fingerdata[3][3].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_013" ) {
            trans = handInfo->m_fingerdata[4][0].direction_mat * trans;
            trans = handInfo->m_fingerdata[4][0].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_014" ) {
            trans = handInfo->m_fingerdata[4][1].direction_mat * trans;
            trans = handInfo->m_fingerdata[4][1].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_015" ) {
            trans = handInfo->m_fingerdata[4][2].direction_mat * trans;
            trans = handInfo->m_fingerdata[4][2].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_016" ) {
            trans = handInfo->m_fingerdata[4][3].direction_mat * trans;
            trans = handInfo->m_fingerdata[4][3].position_mat * trans;
        }
        /*else if ( boneNodeName == "Bone_017" ) {
        }*/
        else if ( boneNodeName == "Bone_018" ) {
            trans = handInfo->m_fingerdata[0][1].direction_mat * trans;
            trans = handInfo->m_fingerdata[0][1].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_019" ) {
            trans = handInfo->m_fingerdata[0][2].direction_mat * trans;
            trans = handInfo->m_fingerdata[0][2].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_020" ) {
            trans = handInfo->m_fingerdata[0][3].direction_mat * trans;
            trans = handInfo->m_fingerdata[0][3].position_mat * trans;
        }



    }

    if(handInfo->m_righthand){out_matrix = trans * modelMesh.invBoneBaseposeMatrixList[i]; }//eigen doushi
    else out_matrix = out_matrix * modelMesh.invBoneBaseposeMatrixList[i];


   // std::cout << i << "\n" << out_matrix << std::endl;
  }
}

Eigen::Matrix4f handFbxLoader::CalculateLocalBoneMatrix(std::string boneNodeName, HandInfo* handInfo){

  /*  auto boneNodeId = this->nodeIdDictionaryAnimation.at(boneNodeName);
    auto boneNode = this->fbxSceneAnimation->GetNode(boneNodeId);
    */
    Eigen::Matrix4f res;
    return res;
}


