#include "eigenutil.h"
#include "mathhelper.h"
#include <algorithm>
#include <math.h>
#include <mathhelper.h>
#include <iostream>


Eigen::Matrix4f QRRUtil::perspective
(
    double fovy,
    double aspect,
    double zNear,
    double zFar
)
{
    //using Matrix4 = Eigen::Matrix<T,4,4>;

    assert(aspect > 0);
    assert(zFar > zNear);

    double radf = QRRUtil::ToRad(fovy);

    double tanHalfFovy = tan(radf / 2.0 );
    Eigen::Matrix4f res = Eigen::Matrix4f::Zero();
    res(0,0) = 1.0 / (aspect * tanHalfFovy);
    res(1,1) = 1.0 / (tanHalfFovy);
    res(2,2) = -(zFar + zNear) / (zFar - zNear);
    res(3,2) = -1.0;
    res(2,3) = -(2.0 * zFar * zNear) / (zFar - zNear);

    return res;


}

Eigen::Matrix4f QRRUtil::lookAt(Eigen::Vector3f const &eye,
                                  Eigen::Vector3f const & center,
                                  Eigen::Vector3f const &up)
{


    Eigen::Vector3f f = (center - eye).normalized();
    Eigen::Vector3f u = up.normalized();
    Eigen::Vector3f s = f.cross(u).normalized();
    u = s.cross(f);

    Eigen::Matrix4f res;

    res << s.x(), s.y(), s.z(), -s.dot(eye),
           u.x(), u.y(), u.z(), -u.dot(eye),
          -f.x(),-f.y(),-f.z(), f.dot(eye),
           0    , 0    , 0    ,  1         ;

    return res;


}

Eigen::Matrix4f QRRUtil::MakeTransform(Eigen::Vector3f const &trans){

   Eigen::Matrix4f res;

   res <<  1 , 0 , 0 , trans.x(),
           0 , 1 , 0 , trans.y(),
           0 , 0 , 1 , trans.z(),
           0 , 0 , 0 , 1        ;
   return res;
}

Eigen::Matrix4f QRRUtil::MakeRotationZAxis(Eigen::Vector3f const &vec){


    Eigen::Vector3f f = vec.normalized();
    std::cout << f << std::endl;
    Eigen::Vector3f u = EigenVector3fMake(0.0f ,1.0f ,0.0f).normalized();
    Eigen::Vector3f s = f.cross(u).normalized();
    u = s.cross(f);

    Eigen::Matrix4f res;

    res << s.x(), s.y(), s.z(), 0.0,
           u.x(), u.y(), u.z(), 0.0,
          -f.x(),-f.y(),-f.z(), 0.0,
           0    , 0    , 0    ,  1         ;

    return res;
}


Eigen::Matrix4f QRRUtil::MakeRotationYAxis(Eigen::Vector3f const &vec){

}

Eigen::Matrix4f QRRUtil::MakeRotationXAxis(Eigen::Vector3f const &vec){
    Eigen::Vector3f f = vec.normalized();
    std::cout << f << std::endl;
    Eigen::Vector3f u = EigenVector3fMake(0.0f ,1.0f ,0.0f).normalized();
    Eigen::Vector3f s = f.cross(u).normalized();
    u = s.cross(f);

    Eigen::Matrix4f res;

    res << s.x(), s.y(), s.z(), 0.0,
           u.x(), u.y(), u.z(), 0.0,
          -f.x(),-f.y(),-f.z(), 0.0,
           0    , 0    , 0    ,  1         ;

    return res;
}



 Eigen::Vector2f QRRUtil::EigenVector2fMake(float const x, float const y)
{
  Eigen::Vector2f v = {x, y};
  return std::move(v);
}


 Eigen::Vector3f QRRUtil::EigenVector3fMake(float const x, float const y, float const z)
{
  Eigen::Vector3f v = {x, y ,z};
  return std::move(v);
}

 Eigen::Vector4f QRRUtil::EigenVector4fMake(float const x, float const y, float const z, float const w)
{
  Eigen::Vector4f v = {x, y, z, w};
  return std::move(v);
}

Eigen::Vector2f QRRUtil::EivenVector4fNormalize(Eigen::Vector2f &&vec){
}

Eigen::Vector3f QRRUtil::EivenVector4fNormalize(Eigen::Vector3f &&vec){
}

Eigen::Vector4f QRRUtil::EivenVector4fNormalize(Eigen::Vector4f &&vec){
   // double total=0;

   // std::for_each(vec. vec.end(), [=](double x) -> void {total += x;});


  //     total += total;



}

 Eigen::Vector2d QRRUtil::EigenVector2dMake(double const x, double const y)
{
  Eigen::Vector2d v = {x, y};
  return std::move(v);
}


 Eigen::Vector3d QRRUtil::EigenVector3dMake(double const x, double const y, double const z)
{
  Eigen::Vector3d v = {x, y ,z};
  return std::move(v);
}

 Eigen::Vector4d QRRUtil::EigenVector4dMake(double const x, double const y, double const z, double const w)
{
  Eigen::Vector4d v = {x, y, z, w};
  return std::move(v);
}

Eigen::Vector2d QRRUtil::EivenVector4dNormalize(Eigen::Vector2d &&vec){
}

Eigen::Vector3d QRRUtil::EivenVector4dNormalize(Eigen::Vector3d &&vec){
}

Eigen::Vector4d QRRUtil::EivenVector4dNormalize(Eigen::Vector4d &&vec){

}

Eigen::Matrix4f QRRUtil::MakeMatrixfromQuat(float x, float y, float z, float w){
    Eigen::Matrix4f ret;
    ret << 1 - 2*y*y - 2*z*z , 2*x*y + 2*w*z     , 2*x*z - 2*w*y     , 0.0f ,
           2*x*y - 2*w*z     , 1 - 2*x*x - 2*z*z , 2*y*z + 2*w*x     , 0.0f ,
           2*x*z + 2*w*y     , 2*y*z - 2*w*x     , 1 - 2*x*x - 2*y*y , 0.0f ,
           0                 , 0                 , 0                 , 1.0f;
    return ret;
}

Eigen::Vector3f QRRUtil::MakeEulerYRotationVec( Eigen::Vector3f rvec ,float theta){

    Eigen::Matrix3f mat;
    mat <<  cos(theta),       0,  sin(theta) ,
                     0,       1,           0 ,
           -sin(theta),       0,  cos(theta) ;

    Eigen::Vector3f ret;

    ret = mat * rvec;

    return ret;
}


Eigen::Vector3f QRRUtil::MakeEulerRotationVec(Eigen::Vector3f bvec, Eigen::Vector3f rvec ,float theta){

    auto x = bvec.x();
    auto y = bvec.y();
    auto z = bvec.z();

    Eigen::Matrix3f mat;
    mat << x*x*(1-cos(theta)) + cos(theta)    , x*y*(1-cos(theta)) - z*sin(theta) , z*x*(1-cos(theta)) + y*sin(theta) ,
           x*y*(1-cos(theta)) + z * sin(theta), y*y*(1-cos(theta)) + cos(theta)   , y*z*(1-cos(theta)) - x*sin(theta) ,
           z*x*(1-cos(theta)) - y * sin(theta), y*z*(1-cos(theta)) + x*sin(theta) , z*z*(1-cos(theta)) + cos(theta)   ;


    Eigen::Vector3f ret;

    ret = mat * rvec;

    return ret;
}


Eigen::Matrix4f QRRUtil::MakeEulerYRotationMat( float theta){

    Eigen::Matrix4f mat;
    mat <<  cos(theta),       0,  sin(theta) ,0,
                     0,       1,           0 ,0,
           -sin(theta),       0,  cos(theta) ,0,
            0,0,0,1;

    return mat;
}


Eigen::Matrix4f QRRUtil::MakeEulerRotationMat(Eigen::Vector3f bvec,float theta){

    auto x = bvec.x();
    auto y = bvec.y();
    auto z = bvec.z();

    Eigen::Matrix4f mat;
    mat << x*x*(1-cos(theta)) + cos(theta)    , x*y*(1-cos(theta)) - z*sin(theta) , z*x*(1-cos(theta)) + y*sin(theta) ,0,
           x*y*(1-cos(theta)) + z * sin(theta), y*y*(1-cos(theta)) + cos(theta)   , y*z*(1-cos(theta)) - x*sin(theta) ,0,
           z*x*(1-cos(theta)) - y * sin(theta), y*z*(1-cos(theta)) + x*sin(theta) , z*z*(1-cos(theta)) + cos(theta)   ,0,
            0,0,0,1;

    return mat;
}






