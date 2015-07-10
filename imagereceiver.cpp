#include "imagereceiver.h"

ImageReceiver::ImageReceiver()
{

}

ImageReceiver::~ImageReceiver()
{

}

std::vector<uchar> ImageReceiver::getImageData()
{

    using boost::asio::ip::udp;

    video_ibuff.clear();

    boost::asio::io_service io_service;
    udp::socket socket(io_service, udp::endpoint(udp::v4(), 1081));

    boost::array<char,65500> recv_buf;
    udp::endpoint remote_endpoint;
    boost::system::error_code error;
    size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);

    for(int i=0; i<recv_buf.size() ; i++){
        video_ibuff.push_back((uchar)recv_buf.data()[i]);
    }

    return video_ibuff;
}

