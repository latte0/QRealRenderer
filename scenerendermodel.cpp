#include "scenerendermodel.h"


SceneRenderModel::SceneRenderModel()
{

}

SceneRenderModel::~SceneRenderModel()
{

}

Eigen::Matrix4f SceneRenderModel::getStateMat()
{
    return m_stateMat;
}

Eigen::Matrix4f SceneRenderModel::getRotationMat()
{
    Eigen::Matrix4f m_rotationMat;
    m_rotationMat << 0, 0, 0, 0,
                     0, 0, 0, 0,
                     0, 0, 0, 0,
                     0, 0, 0, 0;
    return m_rotationMat;
}

Eigen::Matrix4f SceneRenderModel::getTransMat()
{
    Eigen::Matrix4f m_transMat;
    m_transMat << 0, 0, 0, 0,
                     0, 0, 0, 0,
                     0, 0, 0, 0,
                     0, 0, 0, 0;
    return m_transMat;
}
