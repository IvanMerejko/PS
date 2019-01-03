#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QFileDialog>
#include <QWheelEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->saveImage, &QPushButton::clicked , [&](){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                        QString());
        ui->label->save(fileName);
    });

    connect(ui->loadImage , &QPushButton::clicked , [&](){
        ui->label->loadPixmapData(QFileDialog::getOpenFileName(this, tr("Open File"),
                                                               "/home",
                                                               tr("Images (*.png *.xpm *.jpg *.jpeg)")));
        ui->label->getMiddleColor(2);
        ui->label->getMiddleColor(1);
        ui->label->getMiddleColor(0);

        ui->label->setLimits(ui->label->redLimits , 2);
        ui->label->setLimits(ui->label->greenLimits , 1);
        ui->label->setLimits(ui->label->blueLimits , 0);

        ui->redPixel->setColor(2);
        ui->redPixel->setImage(ui->label->getImage());
        ui->redPixel->update();

        ui->bluePixel->setColor(0);
        ui->bluePixel->setImage(ui->label->getImage());
        ui->bluePixel->update();

        ui->greenPixel->setColor(1);
        ui->greenPixel->setImage(ui->label->getImage());
        ui->greenPixel->update();

        ui->red->setValue(ui->label->middleRed);
        ui->blue->setValue(ui->label->middleBlue);
        ui->green->setValue(ui->label->middleGreen);
    });

    connect(ui->red, &QSlider::valueChanged,[this](const int& val)->void{
        ui->label->swapRGB(val , Image::getLimits , 2);
        ui->label->setRed(val);
        ui->redPixel->setImage(ui->label->getImage());
        ui->redPixel->update();

   });
    connect(ui->green, &QSlider::valueChanged,[this](const int& val)->void{
        ui->label->swapRGB(val , Image::getLimits , 1);
        ui->label->setRgreen(val);
        ui->greenPixel->setImage(ui->label->getImage());
        ui->greenPixel->update();
   });
    connect(ui->blue, &QSlider::valueChanged,[this](const int& val)->void{
        ui->label->swapRGB(val , Image::getLimits , 0);
        ui->label->setRblue(val);
        ui->bluePixel->setImage(ui->label->getImage());
        ui->bluePixel->update();

   });


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow :: keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
}
