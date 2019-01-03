#include "workwithimage.h"

#include <QLabel>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QEvent>
#include <QGraphicsPixmapItem>
#include <QSlider>
#include <iostream>
workWithImage :: workWithImage(QWidget *parent  )
    : QLabel(parent)  {
}
////////////////////////// OPENCV//////////////////////////////

cv::Mat workWithImage::QtPixmapToCvMat(const QPixmap &pixmap , bool inCloneImageData){
    return QtImageToCvMat(pixmap.toImage() , inCloneImageData);
}
cv::Mat workWithImage::QtImageToCvMat(const QImage &image, bool inCloneImageData){
    switch ( image.format() ){
             // 8-bit, 4 channel
             case QImage::Format_ARGB32:
             case QImage::Format_ARGB32_Premultiplied:{
                cv::Mat  mat( image.height(), image.width(),
                              CV_8UC4,
                              const_cast<uchar*>(image.bits()),
                              static_cast<size_t>(image.bytesPerLine())
                              );
                return (inCloneImageData ? mat.clone() : mat);
             }

             // 8-bit, 3 channel
             case QImage::Format_RGB32:{
                cv::Mat  mat( image.height(), image.width(),
                              CV_8UC4,
                              const_cast<uchar*>(image.bits()),
                              static_cast<size_t>(image.bytesPerLine())
                              );

                cv::Mat  matNoAlpha;
                cv::cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel
                return matNoAlpha;
             }

             // 8-bit, 3 channel
             case QImage::Format_RGB888:{
                QImage   swapped = image.rgbSwapped();
                return cv::Mat( swapped.height(), swapped.width(),
                                CV_8UC3,
                                const_cast<uchar*>(swapped.bits()),
                                static_cast<size_t>(swapped.bytesPerLine())
                                ).clone();
             }

             // 8-bit, 1 channel
             case QImage::Format_Indexed8:{
                cv::Mat  mat( image.height(), image.width(),
                              CV_8UC1,
                              const_cast<uchar*>(image.bits()),
                              static_cast<size_t>(image.bytesPerLine())
                              );

                return (inCloneImageData ? mat.clone() : mat);
             }

             default:
                break;
          }

    return cv::Mat();
}

QImage workWithImage::CvMatToQtImage(const cv::Mat &inMat){
    switch ( inMat.type() )
         {
    // 8-bit, 4 channel
             case CV_8UC4:{
                QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_ARGB32 );

                return image;
             }

             // 8-bit, 3 channel
             case CV_8UC3:{
                QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_RGB888 );

                return image.rgbSwapped();
             }
        // 8-bit, 1 channel
             case CV_8UC1:{
                QImage image( inMat.data,
                               inMat.cols, inMat.rows,
                               static_cast<int>(inMat.step),
                               QImage::Format_Grayscale8 );
                  return image;
             }
            default:
            break;
      }

      return QImage();


}
QPixmap workWithImage::CvMatToQTPixmap(const cv::Mat &inMat){
    return QPixmap::fromImage( CvMatToQtImage( inMat ) );
}

void workWithImage::setImage(QImage image)
{
    this->image = image;
}

void workWithImage::setColor(int color)
{
    this->color = color;
}
////////////////////////////////////////////////////////////////
void workWithImage::paintEvent(QPaintEvent * const event){
    QPainter painter(this);
    cv::Mat oneColor = workWithImage::QtImageToCvMat(this->image);
    for(int i{0} ; i < oneColor.rows ; ++i){
        for(int j{0} ; j < oneColor.cols ; ++j){
            cv::Vec3b& bgr = oneColor.at<cv::Vec3b>(i, j);
            switch (this->color) {
            case 0:
                bgr[1] = cv::saturate_cast<uchar> (0);
                bgr[2] = cv::saturate_cast<uchar> (0);
                break;
            case 1:
                bgr[0] = cv::saturate_cast<uchar> (0);
                bgr[2] = cv::saturate_cast<uchar> (0);
                break;
            case 2:
                bgr[1] = cv::saturate_cast<uchar> (0);
                bgr[0] = cv::saturate_cast<uchar> (0);
                break;
            default:
                break;
            }
        }
    }
    QImage tmp = workWithImage::CvMatToQtImage(oneColor);
    painter.drawImage(QPoint((size().width() - tmp.width()) / 2 ,(size().height() - tmp.height()) / 2 ) , tmp);

    QLabel::paintEvent(event);
}
