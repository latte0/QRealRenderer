#include "cvutil.h"
#include <QDebug>

QImage QRR::CV::cvMatToQImage( const cv::Mat &inMat )
{
     switch ( inMat.type() )
     {

        case CV_8UC4:
        {
           QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

           return image;
        }


        case CV_8UC3:
        {
           QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

           return image.rgbSwapped();
        }


        case CV_8UC1:
        {
           static QVector<QRgb>  sColorTable;

           if ( sColorTable.isEmpty() )
           {
              for ( int i = 0; i < 256; ++i )
                 sColorTable.push_back( qRgb( i, i, i ) );
           }

           QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

           image.setColorTable( sColorTable );

           return image;
        }

        default:
           qWarning() << "QRR::CV::cvMatToQImage()" << inMat.type();
           break;
     }

     return QImage();
}

QPixmap QRR::CV::cvMatToQPixmap( const cv::Mat &inMat )
{
     return QPixmap::fromImage( cvMatToQImage( inMat ) );
}
