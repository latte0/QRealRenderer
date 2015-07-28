#include "mouserenderer.h"

MouseRenderer::MouseRenderer(QWindow* window) :
    WindowRenderer(window),
    m_mousetex(nullptr)
{
 m_windowpos = QRRUtil::EigenVector2fMake(0.0f,0.0f);
}

MouseRenderer::~MouseRenderer()
{
    delete m_mousetex;
}

void MouseRenderer::update()
{
    switch(mode){
        case MouseMode::OnWindow:
            m_basis = m_attendant->getBasis();
            m_position = m_attendant->getPosition() + m_windowpos.x() * m_attendant->getRightVec()  +  m_windowpos.y() * m_attendant->getDownVec();
            m_position -= m_attendant->getVertVec()*0.000001;
            break;
        case MouseMode::Sphere:
            break;
    }

}

void MouseRenderer::inittex(QOpenGLContext* share)
{
    m_mousetex = new QOpenGLTexture(QImage("./resources/mouse.png"));
    m_mousetex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_mousetex->setMagnificationFilter(QOpenGLTexture::Linear);
}


void MouseRenderer::bindTex()
{
    m_mousetex->bind();
}

void MouseRenderer::collide(Eigen::Vector3f top)
{

}

void MouseRenderer::setWindowPos(float x, float y){
   m_windowpos[0] = x;
   m_windowpos[1] = y;
}

void MouseRenderer::setWindowPos(Eigen::Vector2f pos){
    m_windowpos = pos;
}

void MouseRenderer::setAttendant(WindowRenderer *attendant){
    m_attendant = attendant;
}

Eigen::Vector2f MouseRenderer::getWindowPos(){
   return m_windowpos;
}




