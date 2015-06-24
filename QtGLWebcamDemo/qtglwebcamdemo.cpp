#include "qtglwebcamdemo.h"
#include "ui_qtglwebcamdemo.h"

QtGLWebcamDemo::QtGLWebcamDemo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtGLWebcamDemo)
{
    ui->setupUi(this);

    mFlipVert=false;
    mFlipHoriz=false;
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

    if( mFlipVert && mFlipHoriz )
        cv::flip( image,image, -1);
    else if( mFlipVert )
        cv::flip( image,image, 0);
    else if( mFlipHoriz )
        cv::flip( image,image, 1);


    // Do what you want with the image :-)

    // Show the image
    ui->openCVviewer->showImage( image );
}

void QtGLWebcamDemo::on_actionVertical_Flip_triggered(bool checked)
{
    mFlipVert = checked;
}

void QtGLWebcamDemo::on_action_Horizontal_Mirror_triggered(bool checked)
{
    mFlipHoriz = checked;
}
