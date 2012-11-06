/* 
 * File:   retargeting.cpp
 * Author: Andrew Tilisky
 * 
 * Created on November 2, 2012, 10:41 PM
 */

#include "Retargeting.h"
#include <qpixmap.h>
#include <iostream>
#include <sstream>
#include <cstdio>

using namespace std;

Retargeting::Retargeting()
{
}

Retargeting::Retargeting(const Retargeting& orig)
{
}

Retargeting::~Retargeting()
{
}

bool Retargeting::setImagePath(string path)
{
    imagePath = path;
    return true;
}

bool Retargeting::setImage(string path)
{

    QString QsPath;
    cout << "trying to set " << path << " as image for retarget" << endl;
    image.load(QsPath.fromStdString(path));
    cout << "format is " << image.format() << endl;
    return true;
}

bool Retargeting::setImage()
{
    QString QsPath;
    cout << "trying to set " << imagePath << " as image for retarget" << endl;
    image.load(QsPath.fromStdString(imagePath));

    // De-Allocate memory to prevent memory leak
    for (int i = 0; i < image.width(); ++i)
        delete [] vertSeams[i];
    delete [] vertSeams;

    // De-Allocate memory to prevent memory leak
    for (int i = 0; i < image.height(); ++i)
        delete [] latSeams[i];
    delete [] latSeams;

    cout << "format is " << image.format() << endl;
    return true;
}

string Retargeting::getImagePath()
{
    return imagePath;
}

QImage Retargeting::getImage()
{
    return image;
}

QImage Retargeting::setEFunc()
{
    int n = image.width();
    int m = image.height();

    cout << "copying.  width = " << image.width() << " height = " << image.height() << endl;
    eFunc = image.copy(0, 0, image.width(), image.height());
    QRgb px;

    int upComp;
    int downComp;
    int leftComp;
    int rightComp;

    int uppx;
    int downpx;
    int leftpx;
    int rightpx;

    int energy;
    int maxenergy;

    for(int i = 1; i < n - 1; i++)
    {
        for(int j = 1; j < m - 1; j++)
        {
            upComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i - 1, j));
            downComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i + 1, j));
            leftComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i, j - 1));
            rightComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i , j + 1));

            energy = abs(upComp + downComp + leftComp + rightComp);

            if(eFunc.format() != 3)
            {
                eFunc.setPixel(i, j, qRgb(energy, energy, energy));
            }else
            {
                eFunc.setPixel(i, j, energy);
            }
        }
    }
    //cout << "maxenergy = " << maxenergy / (m * n) << endl;
    return eFunc;
}

void Retargeting::verticalSeams()
{
    int n = image.width();
    int m = image.height();

    // Allocate memory
    vertSeams = new double*[n];

    for (int i = 0; i < n; ++i)
        vertSeams[i] = new double[m];

    for(int i = 1; i < n; i++)
    {
        for(int j = 1; j < m - 1; j++)
        {
            int upLeft = qGray(eFunc.pixel(i - 1, j - 1));
            int up = qGray(eFunc.pixel(i - 1, j));
            int upRight = qGray(eFunc.pixel(i - 1, j + 1));

            vertSeams[i][j] = qGray(eFunc.pixel(i, j)) + min(upLeft, min(up, upRight));
        }
    }
}

void Retargeting::horizontalSeams()
{
    int n = image.width();
    int m = image.height();

    // Allocate memory
    latSeams = new double*[n];

    for (int i = 0; i < n; ++i)
        latSeams[i] = new double[m];

    for(int j = 1; j < m; j++)
    {
        for(int i = 1; i < n - 1; i++)
        {
            int leftDown = qGray(eFunc.pixel(i - 1, j - 1));
            int left = qGray(eFunc.pixel(i, j - 1));
            int leftUp = qGray(eFunc.pixel(i + 1, j - 1));

            vertSeams[i][j] = qGray(eFunc.pixel(i, j)) + min(leftDown, min(left, leftUp));
        }
    }
}

QImage Retargeting::getEFunc()
{
    return eFunc;
}
