#ifndef HANDFBXLOADER_H_
#define HANDFBXLOADER_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <fbxsdk.h>

#include <Eigen/Core>
#include <Eigen/Geometry>
//#include <Eigen/Dense>
#include "eigenutil.h"
#include "external/Leap.h"


#include "fbxloader.h"
#include "handinfo.h"


class handFbxLoader : public fbxLoader
{
public:

    handFbxLoader();
    ~handFbxLoader();

    void GetBoneMatrix(float frame, int meshId, Eigen::Matrix4f* out_matrixList, int matrixCount);

    Eigen::Matrix4f CalculateLocalBoneMatrix(std::string boneNode);


};

#endif
