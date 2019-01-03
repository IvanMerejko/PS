#pragma once

#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QEvent>
#include <QList>
#include <QSlider>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <bitset>
QT_FORWARD_DECLARE_CLASS(QPaintEvent)
typedef char byte;
typedef std::map<std::pair<int , int> , std::pair<double , double>>  coefficient;
typedef std::pair<double , double>  TwoDouble;
class Image : public QLabel
{
    Q_OBJECT
public:
    coefficient redLimits;
    coefficient greenLimits;
    coefficient blueLimits;

    int middleRed = 0;
    int middleBlue = 0 ;
    int middleGreen = 0;
public:
    explicit Image(QWidget *parent = Q_NULLPTR);
    QPoint getCenter(QPixmap &pixmap) ;
    QPixmap& getTmp();
    QImage getImage();
    void setLimits(coefficient & , int );

    void setRed(int value);
    void setRgreen(int value);
    void setRblue(int value);
    void swapRGB(const int & ,  TwoDouble( *)(int , int , coefficient &) , int );
    bool check(int , int);
    int getMiddleColor(int color);
    static TwoDouble getLimits(int i, int j , coefficient &allLimits);

    void save(QString fileName);
public slots:
    void setScaledPixmap(const QPixmap & pixmap);
    void loadPixmapData(const QString &path);

protected:
    void paintEvent(QPaintEvent * const);
    void wheelEvent(QWheelEvent * const) Q_DECL_OVERRIDE;

private:
    QPixmap myPixmap;
    QPixmap tmp;
    int scale = 0;



    int Rred = 0;
    int Rgreen = 0;
    int Rblue = 0;

};

