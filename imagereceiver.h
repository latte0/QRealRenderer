#ifndef IMAGERECEIVER_H
#define IMAGERECEIVER_H

#include <QImage>
#include <QThread>

#include <math.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include <highgui.hpp>
#include <core.hpp>
#include <imgproc.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>


class ImageReceiver : public QObject
{
    Q_OBJECT

public:
    ImageReceiver(int port,std::mutex *mtx);
    ~ImageReceiver();


    cv::Mat getImageData();
    std::mutex* getMutex();



public slots:
    void run();



private:

    int m_port;
    int m_sock, m_numrcv;
    struct sockaddr_in m_addr;

    std::mutex *m_mtx;

    cv::Mat m_videoImage;
    static constexpr int m_videoSize = 65500;
    static char buff[m_videoSize];

    std::vector<uchar> video_ibuff;

};

#endif // IMAGERECEIVER_H
