#ifndef WORKWHITHIMAGE_H
#define WORKWHITHIMAGE_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QEvent>
#include <QList>
#include <QSlider>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>


class workWithImage : public QLabel
{
    Q_OBJECT
public:
    explicit workWithImage(QWidget *parent = Q_NULLPTR);
    static cv::Mat QtPixmapToCvMat(const QPixmap &pixmap ,bool inCloneImageData = true);
    static cv::Mat QtImageToCvMat(const QImage  &image , bool inCloneImageData = true );
    static QImage  CvMatToQtImage(const cv::Mat &inMat);
    static QPixmap CvMatToQTPixmap(const cv::Mat &inMat);
    void setImage(QImage );
    void setColor(int);
protected:
    void paintEvent(QPaintEvent * const);
private:
    QImage image;
    int color;
};

#endif // WORKWHITHIMAGE_H
