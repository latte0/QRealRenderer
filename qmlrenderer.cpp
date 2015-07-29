#include "qmlrenderer.h"
#include <QPainter>

qmlRenderer::qmlRenderer() :
    WindowRenderer(),
    m_qmltex(nullptr)
{

}

qmlRenderer::~qmlRenderer(){
}

void qmlRenderer::inittex(std::shared_ptr<QOpenGLContext>& share){

    m_rwindow = new RenderWindow(m_mutex, m_filename);
    m_rwindow->init();


    //initialize scene graph
   // m_rwindow->show();
   // m_rwindow->hide();


}

void qmlRenderer::bindTex(){

    QImage image = QImage(m_rwindow->getQmlSize(), QImage::Format_ARGB32_Premultiplied);
    QPainter imagePainter(&image);

    m_mutex->lock();
        imagePainter.setRenderHint(QPainter::Antialiasing, true);
        imagePainter.drawImage(0,0,m_rwindow->qmlimage.mirrored());
        imagePainter.setPen(QPen(Qt::red, 12, Qt::DashDotLine, Qt::RoundCap));

        if(m_touched)imagePainter.drawEllipse(touch_x, touch_y, 30, 30);

        m_qmltex.reset(new QOpenGLTexture(image));
        m_qmltex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_qmltex->setMagnificationFilter(QOpenGLTexture::Linear);

    m_mutex->unlock();

    m_qmltex->bind();

}

void qmlRenderer::update(){
    setAspect( (float )getQmlSize().width() / (float)getQmlSize().height());
}

QSize qmlRenderer::getQmlSize(){

    return m_rwindow->getQmlSize();
}

void qmlRenderer::collide(Eigen::Vector3f top){



   // int qmlwidth = m_rwindow->m_rootItem->width(), qmlheigt = m_rwindow->m_rootItem->height();
    int qmlwidth = getQmlSize().width(), qmlheight = getQmlSize().height();

    auto pos = calcPos(top);

    touch_x = pos.x()*qmlwidth; touch_y = (qmlheight-pos.y()*qmlheight);

    auto insideRect = [&pos](const float length) -> bool
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
