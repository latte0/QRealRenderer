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


void handFbxLoader::GetBoneMatrix(float frame, int meshId, Matrix4f* out_matrixList, int matrixCount)
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
    if(HandInfo::singleton().m_righthand){


        if ( boneNodeName == "Bone" ) {
          //  trans = HandInfo::singleton().m_handGlobalNorm;
           // trans = QRRUtil::MakeTransform(HandInfo::singleton().m_handGlobalPos) * trans;
               if(boneNode->GetParent()->GetParent() != NULL){
                   //std::cout << "tashika ni tsuzuki ga aru !!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                FbxNodeAttribute* boneAttributea = boneNode->GetParent()->GetParent()->GetNodeAttribute();

             //   qDebug() << "a";
           //     std::cout << boneAttributea->GetAttributeType() << std::endl;
            }
        //    else std::cout << "tsuzuki nashi " << std::endl;
        }
        else if ( boneNodeName == "Bone_001" ) {
            trans = HandInfo::singleton().m_fingerdata[1][0].position_mat * HandInfo::singleton().m_fingerdata[1][0].direction_mat;

        }
        else if ( boneNodeName == "Bone_002" ) {
         //   Eigen::Vector3f v = getSkeleton("Bone_001")->LimbLength * HandInfo::singleton().m_fingerdata[1][0].direction;

            trans = HandInfo::singleton().m_fingerdata[1][1].position_mat * HandInfo::singleton().m_fingerdata[1][1].direction_mat;
        }
        else if ( boneNodeName == "Bone_003" ) {
           // Eigen::Vector3f v = getSkeleton("Bone_001")->LimbLength * HandInfo::singleton().m_fingerdata[1][0].direction +   getSkeleton("Bone_002")->LimbLength * HandInfo::singleton().m_fingerdata[1][1].direction;

            trans = HandInfo::singleton().m_fingerdata[1][2].position_mat * HandInfo::singleton().m_fingerdata[1][2].direction_mat;
        }
        else if ( boneNodeName == "Bone_004" ) {
           // Eigen::Vector3f v = getSkeleton("Bone_001")->LimbLength * HandInfo::singleton().m_fingerdata[1][0].direction +   getSkeleton("Bone_002")->LimbLength * HandInfo::singleton().m_fingerdata[1][1].direction +  getSkeleton("Bone_003")->LimbLength * HandInfo::singleton().m_fingerdata[1][2].direction;

            trans = HandInfo::singleton().m_fingerdata[1][3].position_mat * HandInfo::singleton().m_fingerdata[1][3].direction_mat;
        }

        else if ( boneNodeName == "Bone_005" ) {

            trans = HandInfo::singleton().m_fingerdata[2][0].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[2][0].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_006" ) {
            trans = HandInfo::singleton().m_fingerdata[2][1].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[2][1].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_007" ) {
            trans = HandInfo::singleton().m_fingerdata[2][2].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[2][2].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_008" ) {
            trans = HandInfo::singleton().m_fingerdata[2][3].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[2][3].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_009" ) {
            trans = HandInfo::singleton().m_fingerdata[3][0].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[3][0].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_010" ) {
            trans = HandInfo::singleton().m_fingerdata[3][1].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[3][1].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_011" ) {
            trans = HandInfo::singleton().m_fingerdata[3][2].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[3][2].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_012" ) {
            trans = HandInfo::singleton().m_fingerdata[3][3].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[3][3].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_013" ) {
            trans = HandInfo::singleton().m_fingerdata[4][0].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[4][0].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_014" ) {
            trans = HandInfo::singleton().m_fingerdata[4][1].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[4][1].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_015" ) {
            trans = HandInfo::singleton().m_fingerdata[4][2].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[4][2].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_016" ) {
            trans = HandInfo::singleton().m_fingerdata[4][3].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[4][3].position_mat * trans;
        }
        /*else if ( boneNodeName == "Bone_017" ) {
        }*/
        else if ( boneNodeName == "Bone_018" ) {
            trans = HandInfo::singleton().m_fingerdata[0][1].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[0][1].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_019" ) {
            trans = HandInfo::singleton().m_fingerdata[0][2].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[0][2].position_mat * trans;
        }
        else if ( boneNodeName == "Bone_020" ) {
            trans = HandInfo::singleton().m_fingerdata[0][3].direction_mat * trans;
            trans = HandInfo::singleton().m_fingerdata[0][3].position_mat * trans;
        }



    }

    if(HandInfo::singleton().m_righthand){out_matrix = trans * modelMesh.invBoneBaseposeMatrixList[i]; }//eigen doushi
    else out_matrix = out_matrix * modelMesh.invBoneBaseposeMatrixList[i];


   // std::cout << i << "\n" << out_matrix << std::endl;
  }
}

Eigen::Matrix4f handFbxLoader::CalculateLocalBoneMatrix(std::string boneNodeName){

  /*  auto boneNodeId = this->nodeIdDictionaryAnimation.at(boneNodeName);
    auto boneNode = this->fbxSceneAnimation->GetNode(boneNodeId);
    */
    Eigen::Matrix4f res;
    return res;
}


