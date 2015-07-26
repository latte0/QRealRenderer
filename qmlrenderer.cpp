#include "qmlrenderer.h"
#include <QPainter>

qmlRenderer::qmlRenderer() :
    WindowRenderer(),
    m_qmltex(nullptr)
{

}

qmlRenderer::~qmlRenderer(){
        delete m_qmltex;
}

void qmlRenderer::inittex(QOpenGLContext * share){

    m_rwindow = new RenderWindow(m_mutex, m_filename);
    m_rwindow->init();
    m_rwindow->resize(960, 1080);
    m_rwindow->show();


}

void qmlRenderer::bindTex(){

    QImage image = QImage(1000, 1000, QImage::Format_ARGB32_Premultiplied);
    QPainter imagePainter(&image);

    m_mutex->lock();
       imagePainter.setRenderHint(QPainter::Antialiasing, true);
        imagePainter.drawImage(0,0,m_rwindow->qmlimage.mirrored());
        imagePainter.setPen(QPen(Qt::red, 12, Qt::DashDotLine, Qt::RoundCap));
        if(m_touched)imagePainter.drawEllipse(touch_x-10, touch_y-10, 30, 30);

        if(m_qmltex != nullptr) delete m_qmltex;
        m_qmltex = new QOpenGLTexture(image);
        m_qmltex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_qmltex->setMagnificationFilter(QOpenGLTexture::Linear);

    m_mutex->unlock();

    m_qmltex->bind();

}

void qmlRenderer::update(){

}

void qmlRenderer::collide(Eigen::Vector3f top){


   // int qmlwidth = m_rwindow->m_rootItem->width(), qmlheigt = m_rwindow->m_rootItem->height();
    int qmlwidth = 1000, qmlheight = 1000;
    Eigen::Vector2f pos = calcPos(top);

    touch_x = pos.x()*qmlwidth; touch_y = (qmlheight-pos.y()*qmlheight);

    auto insideRect = [&pos](const int length) -> bool
    {
        return ( pos.x() < 1.0  + length&& pos.x() > 0.0 -length &&pos.y() < 1.0 + length && pos.y() > 0.0 -length );
    };


    if( getH(top) > -touchdistance /2.0 && getH(top) < touchdistance * 2 &&
          m_handtouch == true &&
            insideRect(0.5f)  )
    {

        if(insideRect(0.0f) && m_pressed == false){
            QMouseEvent mappedPressEvent(QEvent::MouseButtonPress,  QPointF(qmlwidth* pos.x(), qmlheight * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
            QCoreApplication::sendEvent(m_rwindow->m_quickWindow, &mappedPressEvent);
            m_pressed = true;
        }

        QMouseEvent mappedMoveEvent(QEvent::MouseMove,  QPointF(qmlwidth* pos.x(), qmlheight * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
        QCoreApplication::sendEvent(m_rwindow->m_quickWindow, &mappedMoveEvent);
        m_touched = true;

    }else{
        if(m_touched == true){
            QMouseEvent mappedReleaseEvent(QEvent::MouseButtonRelease,  QPointF(qmlwidth* pos.x(), qmlheight * pos.y()), Qt::LeftButton, Qt::LeftButton,   Qt::NoModifier   );
            QCoreApplication::sendEvent(m_rwindow->m_quickWindow, &mappedReleaseEvent);
            m_touched = false;
            m_pressed = false;
        }
    }

}
