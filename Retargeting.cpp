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
#include <fstream>

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
    /*
    //delete &imagePath;
    cout << "imagepath deleted" << endl;
    //delete &energyFunction;
    cout << "energyFunction deleted" << endl;
    */

    // De-Allocate memory to prevent memory leak
    for (int i = 0; i < image.width(); ++i)
        delete [] vertSeams[i];
    cout << "vertSeams 2nd dim arrays deleted" << endl;
    delete [] vertSeams;
    cout << "vertSeams deleted" << endl;

    // De-Allocate memory to prevent memory leak
    //for (int i = 0; i < image.height(); ++i)
    //delete [] latSeams[i];

    /*
    cout << "latSeams 2nd dim arrays deleted" << endl;
    //delete [] latSeams;
    cout << "latSeams deleted" << endl;

    //delete &image;
    cout << "image deleted" << endl;
    //delete &energySet;
    cout << "energySet deleted" << endl;
    //delete &imageSet;
    cout << "imageSet deleted" << endl;
    */

    /*
    // De-Allocate memory to prevent memory leak
    for (int i = 0; i < image.width(); ++i)
        delete [] vertPixelsRemoved[i];
    delete [] vertPixelsRemoved;

    // De-Allocate memory to prevent memory leak
    for (int i = 0; i < image.height(); ++i)
        delete [] latPixelsRemoved[i];

    delete [] latPixelsRemoved;
    */
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
    //there are usually m rows in a matrix but QImage uses cartesian coordinates
    //so there are m columns (x-sub-m)
    int m = image.width();

    //vice versa with n (n is y-sub-n)
    int n = image.height();

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

    for (int i = 1; i < m - 1; i++)
    {
        for (int j = 1; j < n - 1; j++)
        {
            upComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i - 1, j));
            downComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i + 1, j));
            leftComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i, j - 1));
            rightComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i , j + 1));

            energy = abs(upComp + downComp + leftComp + rightComp);

            if (energyFunction.format() != 3)
            {
                energyFunction.setPixel(i, j, qRgb(energy, energy, energy));
            }
            else
            {
                energyFunction.setPixel(i, j, energy);
            }
        }
    }
    //cout << "maxenergy = " << maxenergy / (m * n) << endl;
    energySet = true;
    return energyFunction;
}

bool** Retargeting::setVertSeams(int widthDifference)
{
    vector< vector<int > > seamEnergies;


    cout << "vertical seam values" << endl;

    for (int i = 0; i < image.width(); i++)
    {
        cout << vertSeams[image.height() - 1][i] << " ";
    }

    return vertPixelsRemoved;
}

bool** Retargeting::setLatSeams(int heightDifference)
{
    cout << endl << "lateral seam values" << endl;

    for (int i = 0; i < image.height(); i++)
    {
        cout << latSeams[i][image.width() - 1] << " ";
    }
    cout << endl;
    return latPixelsRemoved;
}
/**/

void Retargeting::getVerticalSeamTable()
{
    int m = image.width();
    int n = image.height();

    // Allocate memory
    vertSeams = new int*[m];

    for (int i = 0; i < m; ++i)
    {
        //cout << i << " ";
        vertSeams[i] = new int[n];

        for (int j = 0; j < n; j++)
        {
            vertSeams[i][j] = 0;
        }
    }

    ofstream outFile;
    char outputFilename[] = "C:\\Users\\Andrew\\Documents\\vertseams.txt";

    outFile.open(outputFilename, ios::out);

    if (!outFile) {
      cerr << "Can't open output file " << outputFilename << endl;
      exit(1);
    }

    for (int j = 1; j < n; j++)
    {
        for (int i = 1; i < m - 1; i++)
        {
            int upRight = vertSeams[i + 1][j - 1];
            int up = vertSeams[i][j - 1];
            int upLeft = vertSeams[i - 1][j - 1];


            int pixelEnergy = qGray(energyFunction.pixel(i, j));
            int seamEnergy = pixelEnergy + min(upLeft, min(up, upRight));
            vertSeams[i][j] = seamEnergy;

            outFile << seamEnergy << "(" << pixelEnergy << ")" << " ";
            //cout << seamEnergy << "(" << pixelEnergy << ")" << " ";
        }
        outFile << endl;
        //cout << endl;
    }
}

void Retargeting::getLateralSeamTable()
{
    int m = image.width();
    int n = image.height();

    // Allocate memory
    latSeams = new int*[n];

    for (int i = 0; i < n; ++i)
        latSeams[i] = new int[m];

    for (int j = 1; j < n; j++)
    {
        for (int i = 1; i < m - 1; i++)
        {
            int rightUp = latSeams[i - 1][j - 1];
            int right = latSeams[i - 1][j];
            int rightDown = latSeams[i - 1][j + 1];

            latSeams[i][j] = qGray(energyFunction.pixel(i, j)) + min(rightUp, min(right, rightDown));
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
