#include "handinfo.h"
#include <iostream>
#include <string.h>

HandInfo::HandInfo()
{
    m_PointDiv = 7.8f;
}

HandInfo::~HandInfo()
{

}

Eigen::Matrix4f HandInfo::LeaptoEigenmat(Leap::Matrix lmat){
    Leap::Vector xb = lmat.xBasis;
    Leap::Vector yb = lmat.yBasis;
    Leap::Vector zb = lmat.zBasis;

    Eigen::Matrix4f mat;

    std::cout << lmat << std::endl;

    mat = Eigen::Matrix4f::Identity();
/*
   mat << xb.x, xb.y, xb.z, 0.0,
           yb.x, yb.y, yb.z, 0.0,
           zb.x, zb.y, zb.z, 0.0,
           0.0 , 0.0 , 0.0 , 1.0;
*/
/*
   mat << xb.x, yb.x, zb.x, 0.0,
           xb.y, yb.y, zb.y, 0.0,
           xb.z, yb.z, zb.z, 0.0,
           0.0 , 0.0 , 0.0 , 1.0;
*/
    std::cout << mat << std::endl;

    return mat;
}

bool HandInfo::UpdateInfo(){
    m_righthand =false;
    Leap::Frame frame_leap = m_Leap.frame();

    Leap::HandList hands = frame_leap.hands();
    for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
       // Get the first hand
       const Leap::Hand hand = *hl;
       std::string handType = hand.isLeft() ? "Left hand" : "Right hand";

       if(hand.isRight()){
           Leap::Vector v = hand.palmPosition();
           m_handGlobalPos = RegenUtil::EigenVector3fMake(v.z/m_PointDiv, v.y/m_PointDiv , -v.x/m_PointDiv);

           const Leap::Vector normal = hand.palmNormal();
           const Leap::Vector direction = hand.direction();

           m_pitch = direction.pitch() * Leap::DEG_TO_RAD;
           m_roll = normal.roll() * Leap::DEG_TO_RAD;
           m_yaw = direction.yaw() * Leap::DEG_TO_RAD;

           m_handGlobalNorm << 1,0,0,0,
                               0,cos(-normal.roll()),sin(-normal.roll()),0,
                                0,-sin(-normal.roll()),cos(-normal.roll()),0,
                                0,0,0,1;

           Leap::Arm arm = hand.arm();

           const Leap::FingerList fingers = hand.fingers();
           int i=0;
           for (Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {

             const Leap::Finger finger = *fl;

             for (int b = 0; b < 4; ++b) {
               Leap::Bone::Type boneType = static_cast<Leap::Bone::Type>(b);
               Leap::Bone bone = finger.bone(boneType);

               m_fingerdata[i][b].direction = RegenUtil::EigenVector3fMake(-bone.direction().z, -bone.direction().y, bone.direction().x).normalized();
               std::cout << m_fingerdata[i][b].direction << std::endl;
             //  m_fingerdata[i][b].direction_mat = RegenUtil::MakeRotationZAxis(RegenUtil::EigenVector3fMake(bone.direction().x ,bone.direction().y,bone.direction().z).normalized());

               m_fingerdata[i][b].direction_mat = LeaptoEigenmat(bone.basis());

               m_fingerdata[i][b].position_mat = RegenUtil::MakeTransform(RegenUtil::EigenVector3fMake(bone.prevJoint().z/m_PointDiv,bone.prevJoint().y/m_PointDiv, -bone.prevJoint().x/m_PointDiv));
             }
             i++;
           }

           m_righthand = true;

       }
     }




    return true;
}

