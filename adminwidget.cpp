#include "adminwidget.h"

AdminWidget::AdminWidget(QWidget *parent) : QWidget(parent)
{
    /*
    QVBoxLayout *mainLayout = new QVBoxLayout;

    gameapp = new SceneRender();
    mainLayout->addWidget(gameapp);

    setLayout(mainLayout);
    setWindowTitle(tr("widget administrator"));

    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), gameapp, SLOT(processing()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(capturetimeout()));
    timer->start( 16 );
*/





}

AdminWidget::~AdminWidget()
{

}

void AdminWidget::capturetimeout()
{
    capturesignal();
}

