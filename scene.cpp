#include "scene.h"

Scene::Scene(std::shared_ptr<QOpenGLContext> context)
{
    m_context = context;

    m_eworld = Eigen::Matrix4f::Identity();
}

Scene::~Scene()
{

}

void Scene::update(){

    Player::singleton().update();


    m_uniformVs.lightDirection = QRRUtil::EigenVector4fMake(0.0f, 0.0f, 500.0f, 0.0f);
    m_uniformVs.lightDirection = m_uniformVs.lightDirection.normalized();

    //m_ecamera = Player::singleton().getEyeMat(eye);


    m_eworld =QRRUtil::MakeTransform( Player::singleton().getPosition() + QRRUtil::EigenVector3fMake(0,0,-400/QRR::Environment::mmDiv));


    m_uniformVs.normalMatrix = m_eworld.inverse();
    m_uniformVs.modelViewMatrix = m_ecamera * m_eworld;
    m_uniformVs.projectionMatrix = m_eproj;


    m_frame += 1.0f;

    for( auto& qml : qmls){
        qml->update();
    }

    /*if (m_frame >= m_fbxLoader.GetAnimationEndFrame())
    {
        m_frame -= m_fbxLoader.GetAnimationEndFrame();
    }
    */
}

void Scene::addModel(const ModelData div, const QString filename){

}

void Scene::addQml(const QmlObject&& qmlobj){

    std::shared_ptr<qmlRenderer> qml(new qmlRenderer());
    qml->init(m_context, qmlobj.filename);
    qml->setCondition(qmlobj.scale, qmlobj.pos, qmlobj.euler_x,  qmlobj.euler_y, qmlobj.touched);

    qmls.push_back(qml);
    currentQml = qml;

}

void Scene::addMouse(){

}






