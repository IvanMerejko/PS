#include "image.h"

#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QEvent>
#include <QGraphicsPixmapItem>
#include <QSlider>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include "workwithimage.h"
Image :: Image(QWidget *parent  )
    : QLabel(parent)  {
}
QPoint   Image::getCenter(QPixmap &pixmap)  {
    return QPoint((size().width() - pixmap.width()) / 2 ,(size().height() - pixmap.height()) / 2 );
}
QPixmap& Image::getTmp()  {
    return this->tmp;
}

QImage Image::getImage()
{
    return myPixmap.toImage();
}
int Image::getMiddleColor( int color){
    cv::Mat image = workWithImage::QtPixmapToCvMat(myPixmap);
    int *middle = NULL;
    switch (color) {
    case 0:
        middle = &middleBlue;
        break;
    case 1:
        middle = &middleGreen;
        break;
    case 2:
        middle = &middleRed;
        break;
    default:
        break;
    }
    for(int i{0} ; i < image.rows ; ++i){
        for(int j{0} ; j < image.cols ; ++j){
            cv::Vec3b& bgr = image.at<cv::Vec3b>(i, j);
            *middle += cv::saturate_cast<int>(bgr[color]);
        }
    }
    return (*middle = *middle / (image.rows * image.cols ));
}
void Image::setLimits(coefficient &arrayOfLimits , int color){
    cv::Mat image = workWithImage::QtPixmapToCvMat(myPixmap);
    int *middle = NULL;
    switch (color) {
    case 0:
        middle = &middleBlue;
        break;
    case 1:
        middle = &middleGreen;
        break;
    case 2:
        middle = &middleRed;
        break;
    default:
        break;
    }
    double minus = 0 , plus = 0;
    for(int i{0} ; i < image.rows ; ++i){
        for(int j{0} ; j < image.cols ; ++j){
            cv::Vec3b& bgr = image.at<cv::Vec3b>(i, j);
            plus = ((UCHAR_MAX - cv::saturate_cast<double>(bgr[color])) / (255 - *middle));
            minus = (cv::saturate_cast<double>(bgr[color]) / *middle);
            arrayOfLimits[std::pair<int , int> (i , j)] = std::pair<double , double> (minus , plus);
        }
    }


}

void Image::setRed(int value){
    this->Rred = value;
}
void Image::setRgreen(int value)
{
    this->Rgreen = value;
}
void Image::setRblue(int value)
{
    this->Rblue = value;
}

bool Image::check(int value , int color){
    switch (color) {
    case 0:
        return value > Rblue;
        break;
    case 1:
        return value > Rgreen;
        break;
    case 2:
        return value > Rred;
        break;
    default:
        break;
    }
    return false;
}
void Image::swapRGB(const int &value , TwoDouble( *getLimits)(int , int , coefficient &) , int color){
    cv::Mat help = workWithImage::QtPixmapToCvMat(myPixmap);
    TwoDouble cof;
    int *middle = NULL;
    switch (color) {
    case 0:
        middle = &middleBlue;
        break;
    case 1:
        middle = &middleGreen;
        break;
    case 2:
        middle = &middleRed;
        break;
    default:
        break;
    }
    for(int i{0} ; i < help.rows ; ++i){
        for(int j{0} ; j < help.cols ; ++j){
            cv::Vec3b& bgr = help.at<cv::Vec3b>(i, j);
            switch (color) {
            case 0:
                cof = getLimits(i , j , blueLimits);
                break;
            case 1:
                cof = getLimits(i , j , greenLimits);
                break;
            case 2:
                cof = getLimits(i , j , redLimits);
                break;
            default:
                break;
            }
            if(value > *middle){
                if(check(value , color)){
                    bgr[color] = cv::saturate_cast<uchar>(cv::saturate_cast<double>(bgr[color]) + 2.55 * cof.second);
                }else{
                     bgr[color] = cv::saturate_cast<uchar>(cv::saturate_cast<double>(bgr[color]) - 2.55 *cof.second);
                }

            }else{
                if(check(value , color)){
                    bgr[color] = cv::saturate_cast<uchar>(cv::saturate_cast<double>(bgr[color]) +2.55 * cof.first);
                }else{
                     bgr[color] = cv::saturate_cast<uchar>(cv::saturate_cast<double>(bgr[color]) - 2.55 *cof.first);
                }
            }

        }
    }
    myPixmap = workWithImage::CvMatToQTPixmap(help);
    update();
}

TwoDouble Image::getLimits(int i, int j , coefficient &allLimits)
{
    return allLimits[TwoDouble(i , j)];
}



void Image::save(QString fileName)
{
    if(!fileName.isNull()){
       tmp.toImage().save(fileName);
    }

}


void Image::setScaledPixmap(const QPixmap &pixmap){
    myPixmap = pixmap;
    tmp = myPixmap;
    update();
}
void Image::loadPixmapData(const QString &path){
    QPixmap pixmap(path);
    if(!pixmap.isNull()){
        setScaledPixmap(pixmap);
    }
}
void Image::wheelEvent(QWheelEvent * const events){
    if(!myPixmap.isNull()){
        this->scale += events->delta();
        this->tmp = this->myPixmap.scaled(this->myPixmap.width() + this->scale , myPixmap.height() + this->scale , Qt::KeepAspectRatio);
        update();

    }

}
void Image::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    if (!this->myPixmap.isNull()){
        this->tmp = myPixmap.scaled(this->tmp.size() , Qt::KeepAspectRatio);
        painter.drawPixmap(getCenter(getTmp()), getTmp());
    }

    QLabel::paintEvent(event);
}




/////////////////////////////////////////////////////////////////






