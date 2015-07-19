#include "imagereceiver.h"

ImageReceiver::ImageReceiver(int port,std::mutex *mtx)
{
    m_port = port;
    m_mtx = mtx;
}

ImageReceiver::~ImageReceiver()
{

}

cv::Mat ImageReceiver::getImageData()
{
       return m_videoImage;
}

std::mutex* ImageReceiver::getMutex(){
    return m_mtx;
}

using boost::asio::ip::udp;


void  ImageReceiver::run(){
    while(1){

        video_ibuff.clear();

        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), m_port));

        boost::array<char,m_videoSize> recv_buf;
        udp::endpoint remote_endpoint;
        boost::system::error_code error;
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);


            for(int i=0; i<recv_buf.size() ; i++){
                video_ibuff.push_back((uchar)recv_buf.data()[i]);
            }

        m_mtx->lock();
            m_videoImage = cv::imdecode(cv::Mat(video_ibuff), CV_LOAD_IMAGE_COLOR);
        m_mtx->unlock();
    }
}

