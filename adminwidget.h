#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "GameApplication.h"

class MainWindow;


class AdminWidget : public QWidget
{
   Q_OBJECT
public:
    explicit AdminWidget(QWidget *parent = 0);
    ~AdminWidget();

private:
    MainWindow *mainWindow;
    GameApplication *gameapp;
    QTimer *timer;

signals:
    void capturesignal();

public slots:
    void capturetimeout();

public slots:
};

#endif // ADMINWIDGET_H
