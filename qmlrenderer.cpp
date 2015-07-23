#include "qmlrenderer.h"

qmlRenderer::qmlRenderer() :
    WindowRenderer(),
    m_qmltex(nullptr)
{

}

qmlRenderer::~qmlRenderer(){
        delete m_qmltex;
}

void qmlRenderer::inittex(){

    m_rwindow = new RenderWindow(m_mutex, m_filename);
    m_rwindow->init();
    m_rwindow->resize(960, 1080);
    m_rwindow->show();

}

void qmlRenderer::bindTex(){

    m_mutex->lock();
        if(m_qmltex != nullptr) delete m_qmltex;
        m_qmltex = new QOpenGLTexture(m_rwindow->qmlimage.mirrored());
        m_qmltex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_qmltex->setMagnificationFilter(QOpenGLTexture::Linear);
    m_mutex->unlock();


    m_qmltex->bind();


}

void qmlRenderer::collide(Eigen::Vector3f top){


    int qmlwidth = 1000, qmlheigt = 1000;

    Eigen::Vector2f pos = calcPos(top);


    auto insideRect = [&pos](const int length) -> bool
    {
        return ( pos.x() < 1.0  + length&& pos.x() > 0.0 -length &&pos.y() < 1.0 + length && pos.y() > 0.0 -length );
    };


    if( getH(top) > -touchdistance /1.4 && getH(top) < touchdistance &&
          m_handtouch == true &&
            insideRect(0.5f)   )
    {

        if(insideRect(0.0f) && m_pressed == false){
            QMouseEvent mappedPressEvent(QEvent::MouseButtonPress,  QPointF(qmlwidth* pos.x(), qmlheigt * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
            QCoreApplication::sendEvent(m_rwindow->m_quickWindow, &mappedPressEvent);
            m_pressed = true;
        }

        QMouseEvent mappedMoveEvent(QEvent::MouseMove,  QPointF(qmlwidth* pos.x(), qmlheigt * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
        QCoreApplication::sendEvent(m_rwindow->m_quickWindow, &mappedMoveEvent);
        m_touched = true;

    }else{
        if(m_touched == true){
            QMouseEvent mappedReleaseEvent(QEvent::MouseButtonRelease,  QPointF(qmlwidth* pos.x(), qmlheigt * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
            QCoreApplication::sendEvent(m_rwindow->m_quickWindow, &mappedReleaseEvent);
            m_touched = false;
            m_pressed = false;
        }
    }

}
