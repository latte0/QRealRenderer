#ifndef CVUTIL_H
#define CVUTIL_H

#include <QImage>
#include <QPixmap>

#include <highgui.hpp>
#include <core.hpp>
#include <imgproc.hpp>

namespace QRR{
    namespace CV{
       QImage cvMatToQImage( const cv::Mat &inMat );
       QPixmap cvMatToQPixmap( const cv::Mat &inMat );
    }
}

#endif // CVUTIL_H
