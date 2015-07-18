#include "handinfo.h"
#include "environmentreader.h"
#include <iostream>
#include <string.h>

HandInfo::HandInfo()
{

}

HandInfo::~HandInfo()
{

}

Eigen::Matrix4f HandInfo::LeaptoEigenmat(Leap::Matrix lmat){
    Leap::Vector xb = -lmat.zBasis;
    Leap::Vector yb = lmat.yBasis;
    Leap::Vector zb = lmat.xBasis;

    Eigen::Matrix4f mat;


    mat = Eigen::Matrix4f::Identity();

    mat << xb.x , yb.x ,zb.x, 0 ,
            xb.y , yb.y ,zb.y, 0 ,
            xb.z , yb.z ,zb.z, 0 ,
            0 , 0 ,0, 1 ;

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
           m_handGlobalPos = QRRUtil::EigenVector3fMake(v.x/QRR::Environment::mmDiv, v.y/QRR::Environment::mmDiv , v.z/QRR::Environment::mmDiv);

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

               m_fingerdata[i][b].direction = QRRUtil::EigenVector3fMake(-bone.direction().z, -bone.direction().y, bone.direction().x).normalized();

               m_fingerdata[i][b].direction_mat = LeaptoEigenmat(bone.basis());

               m_fingerdata[i][b].position = QRRUtil::EigenVector3fMake(bone.prevJoint().x/QRR::Environment::mmDiv,bone.prevJoint().y/QRR::Environment::mmDiv, bone.prevJoint().z/QRR::Environment::mmDiv);

               m_fingerdata[i][b].position_mat = QRRUtil::MakeTransform(QRRUtil::EigenVector3fMake(bone.prevJoint().x/QRR::Environment::mmDiv,bone.prevJoint().y/QRR::Environment::mmDiv, bone.prevJoint().z/QRR::Environment::mmDiv));
             }
             i++;
           }

           m_righthand = true;

       }
     }




    return true;
}

