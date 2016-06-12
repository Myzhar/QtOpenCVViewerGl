#include "cqtopencvviewergl.h"

#include <QOpenGLFunctions>
#include <iostream>
#include <opencv2/opencv.hpp>

CQtOpenCVViewerGl::CQtOpenCVViewerGl(QWidget *parent) :
QOpenGLWidget(parent)
{
    mBgColor = QColor::fromRgb(150, 150, 150);
}

void CQtOpenCVViewerGl::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();
    
    float r = ((float)mBgColor.darker().red())/255.0f;
    float g = ((float)mBgColor.darker().green())/255.0f;
    float b = ((float)mBgColor.darker().blue())/255.0f;
    glClearColor(r,g,b,1.0f);
}

void CQtOpenCVViewerGl::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(0, width, -height, 0, 0, 1);
    
    glMatrixMode(GL_MODELVIEW);
    
    updateScene();
}

void CQtOpenCVViewerGl::updateScene()
{
    if( this->isVisible() )
        //updateGL(); obsolete
        update();
}

void CQtOpenCVViewerGl::paintGL()
{
    makeCurrent();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    renderImage();
}

void CQtOpenCVViewerGl::renderImage()
{
    makeCurrent();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (!mRenderQtImg.isNull())
    {
        glLoadIdentity();
        
        QImage image; // the image rendered
        
        glPushMatrix();
        {
            // calculate
            int nw = this->size().width();
            int nh = floor(nw / mImgRatio);
            
            if (nh > this->size().height()){
                nh = this->size().height();
                nw = floor(nh * mImgRatio);
            }
            
            if (nw == mRenderQtImg.width() && nh == mRenderQtImg.height())
                image = mRenderQtImg;
            else
                image = mRenderQtImg.scaled(QSize(nw, nh),
                                            Qt::IgnoreAspectRatio,
                                            Qt::SmoothTransformation);
        
            // ---> Centering image in draw area
            
            glRasterPos2i(floor((this->size().width() - image.width()) / 2),
                          -floor((this->size().height() - image.height()) / 2));
            
            glPixelZoom(1, -1);
            
            glDrawPixels( image.width(), image.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        }
        glPopMatrix();
        
        // end
        glFlush();
    }
}

bool CQtOpenCVViewerGl::showImage(const cv::Mat& image)
{
    mImgRatio = (float)image.cols/(float)image.rows;
    
    if (image.channels() == 3)
        cvtColor(image, mOrigImage, CV_BGR2RGBA);
    else if (mOrigImage.channels() == 1)
        cvtColor(image, mOrigImage, CV_GRAY2RGBA);
    else return false;
    
    mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                          mOrigImage.cols, mOrigImage.rows,
                          mOrigImage.step1(), QImage::Format_RGB32);
    
    updateScene();
    
    return true;
}