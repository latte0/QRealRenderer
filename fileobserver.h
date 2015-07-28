#ifndef FILEOBSERVER_H
#define FILEOBSERVER_H

#include <QObject>
#include <QDebug>
#include <QString>


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>


class FileObserver : public QObject
{
    Q_OBJECT
public:
    FileObserver();
    void setFile(QString filenaem);

    struct stat stat_buf;

    void run();

private:
    QString m_filename;

signals:
    void filechanged();
};

#endif // FILEOBSERVER_H
