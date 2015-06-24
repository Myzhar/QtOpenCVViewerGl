#include "qtglwebcamdemo.h"
#include "ui_qtglwebcamdemo.h"

QtGLWebcamDemo::QtGLWebcamDemo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtGLWebcamDemo)
{
    ui->setupUi(this);
}

QtGLWebcamDemo::~QtGLWebcamDemo()
{
    delete ui;
}

void QtGLWebcamDemo::on_actionStart_triggered()
{
    if( !mCapture.isOpened() )
        if( !mCapture.open( 0 ) )
            return;

    startTimer(0);
}

void QtGLWebcamDemo::timerEvent(QTimerEvent *event)
{
    cv::Mat image;
    mCapture >> image;

    // Do what you want with the image :-)

    // Show the image
    ui->openCVviewer->showImage( image );
}
