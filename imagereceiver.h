#ifndef IMAGERECEIVER_H
#define IMAGERECEIVER_H

#include <iostream>
#include <vector>
#include <string>

#include <highgui.hpp>
#include <core.hpp>
#include <imgproc.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>


class ImageReceiver
{
public:
    ImageReceiver();
    ~ImageReceiver();
    std::vector<uchar> getImageData();


private:
    std::vector<uchar> video_ibuff;
};

#endif // IMAGERECEIVER_H
