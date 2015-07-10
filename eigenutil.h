#ifndef EIGENUTIL
#define EIGENUTIL

#include <Eigen/Core>
#include <Eigen/Geometry>


namespace RegenUtil{
    Eigen::Vector2f EigenVector2fMake(float const x, float const y);
    Eigen::Vector3f EigenVector3fMake(float const x, float const y, float const z);
    Eigen::Vector4f EigenVector4fMake(float const x, float const y, float const z, float const w);

    Eigen::Vector2f EivenVector4fNormalize(Eigen::Vector2f &&vec);
    Eigen::Vector3f EivenVector4fNormalize(Eigen::Vector3f &&vec);
    Eigen::Vector4f EivenVector4fNormalize(Eigen::Vector4f &&vec);

    Eigen::Vector2d EigenVector2dMake(double const x, double const y);
    Eigen::Vector3d EigenVector3dMake(double const x, double const y, double const z);
    Eigen::Vector4d EigenVector4dMake(double const x, double const y, double const z, double const w);

    Eigen::Vector2d EivenVector4dNormalize(Eigen::Vector2d &&vec);
    Eigen::Vector3d EivenVector4dNormalize(Eigen::Vector3d &&vec);
    Eigen::Vector4d EivenVector4dNormalize(Eigen::Vector4d &&vec);

    Eigen::Matrix4f perspective(double fovy, double aspect, double zNear, double zFar);

    Eigen::Matrix4f lookAt(Eigen::Vector3f const &eye,
                                Eigen::Vector3f const & center,
                                Eigen::Vector3f const &up);

    Eigen::Matrix4f MakeTransform(Eigen::Vector3f const &trans);

    Eigen::Matrix4f MakeRotationXAxis(Eigen::Vector3f const &vec);
    Eigen::Matrix4f MakeRotationYAxis(Eigen::Vector3f const &vec);
    Eigen::Matrix4f MakeRotationZAxis(Eigen::Vector3f const &vec);

}

#endif // EIGENUTIL

