/* 
 * File:   retargeting.cpp
 * Author: Andrew Tilisky
 * 
 * Created on November 2, 2012, 10:41 PM
 */

#include "Retargeting.h"
#include <QGraphicsPixmapItem>
#include <iostream>
#include <sstream>
#include <cstdio>

using namespace std;

Retargeting::Retargeting()
{
    energySet = false;
    imageSet = false;
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
    imageSet = true;
    return true;
}

string Retargeting::getImagePath()
{
    return imagePath;
}

QImage Retargeting::getImage()
{
    imageSet = true;
    return image;
}

QImage Retargeting::setEnergy()
{
    int n = image.width();
    int m = image.height();

    cout << "copying.  width = " << image.width() << " height = " << image.height() << endl;
    energyFunction = image.copy(0, 0, image.width(), image.height());

    int upComp;
    int downComp;
    int leftComp;
    int rightComp;

    /*
    QRgb px;
    int uppx;
    int downpx;
    int leftpx;
    int rightpx;
    int maxenergy;
    */

    int energy;

    for(int i = 1; i < n - 1; i++)
    {
        for(int j = 1; j < m - 1; j++)
        {
            upComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i - 1, j));
            downComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i + 1, j));
            leftComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i, j - 1));
            rightComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i , j + 1));

            energy = abs(upComp + downComp + leftComp + rightComp);

            if(energyFunction.format() != 3)
            {
                energyFunction.setPixel(i, j, qRgb(energy, energy, energy));
            }else
            {
                energyFunction.setPixel(i, j, energy);
            }
        }
    }
    //cout << "maxenergy = " << maxenergy / (m * n) << endl;
    energySet = true;
    return energyFunction;
}

/*
QPoint** setVertSeams(int widthDifference)
{
    for(int s = 0; s < widthDifference; s++)
    {

    }
}

QPoint** setLatSeams(int heightDifference)
{

}*/

void Retargeting::setVerticalSeamTable()
{
    int n = image.width();
    int m = image.height();

    // Allocate memory
    vertSeams = new int*[n];

    for (int i = 0; i < n; ++i)
        vertSeams[i] = new int[m];

    for(int i = 1; i < n; i++)
    {
        for(int j = 1; j < m - 1; j++)
        {
            int upLeft = qGray(energyFunction.pixel(i - 1, j - 1));
            int up = qGray(energyFunction.pixel(i - 1, j));
            int upRight = qGray(energyFunction.pixel(i - 1, j + 1));

            vertSeams[i][j] = qGray(energyFunction.pixel(i, j)) + min(upLeft, min(up, upRight));
        }
    }
}

void Retargeting::getHorizontalSeamTable()
{
    int n = image.width();
    int m = image.height();

    // Allocate memory
    latSeams = new int*[n];

    for (int i = 0; i < n; ++i)
        latSeams[i] = new int[m];

    for(int j = 1; j < m; j++)
    {
        for(int i = 1; i < n - 1; i++)
        {
            int leftDown = qGray(energyFunction.pixel(i + 1, j - 1));
            int left = qGray(energyFunction.pixel(i, j - 1));
            int leftUp = qGray(energyFunction.pixel(i - 1, j - 1));

            latSeams[i][j] = qGray(energyFunction.pixel(i, j)) + min(leftDown, min(left, leftUp));
        }
    }
}

bool Retargeting::isEnergySet()
{
    return energySet;
}

bool Retargeting::isImageSet()
{
    return imageSet;
}

QImage Retargeting::getEnergy()
{
    return energyFunction;
}

int** Retargeting::getVertSeams()
{
    return vertSeams;
}

int** Retargeting::getLatSeams()
{
    return latSeams;
}
