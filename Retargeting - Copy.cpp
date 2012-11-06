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

QImage Retargeting::setEFunc(bool grayscale){
    int n = image.width();
    int m = image.height();

    char hexval [8];
    string rgbval;
    int asint;

    int red;
    int green;
    int blue;

    /*
    int upasint;
    int leftasint;
    int downasint;
    int rightasint;
    */

    cout << "copying.  width = " << image.width() << " height = " << image.height() << endl;
    eFunc = image.copy(0, 0, image.width(), image.height());

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            /*
            int difUp = asint;
            int difDown = asint;
            int difLeft = asint;
            int difRight = asint;
            */

            int difUp = eFunc.pixel(i, j);
            int difDown = eFunc.pixel(i, j);
            int difLeft = eFunc.pixel(i, j);
            int difRight = eFunc.pixel(i, j);

            /*
            */
            if(i > 0)
            {
                 difUp = abs(eFunc.pixel(i, j) - eFunc.pixel(i - 1, j));
            }
            if(j > 0)
            {
                 difLeft = abs(eFunc.pixel(i, j) - eFunc.pixel(i, j - 1));
            }
            if(i < n - 1)
            {
                 difDown = abs(eFunc.pixel(i, j) - eFunc.pixel(i + 1, j));
            }
            if(j < m - 1)
            {
                 difRight = abs(eFunc.pixel(i, j) - eFunc.pixel(i, j + 1));
            }

            /*
            if(i > 0)
            {
                //convert int to hex
                sprintf(hexval,"%X", eFunc.pixel(i - 1, j));

                //trunc 1st 2 numbers
                rgbval =((string) hexval).substr(2,6);

                //convert string to hex
                stringstream intstrm;
                intstrm << hex << rgbval;

                //stream to int var
                intstrm >> upasint;

                 difUp = abs(asint - upasint);
            }
            if(j > 0)
            {
                //convert int to hex
                sprintf(hexval,"%X", eFunc.pixel(i, j - 1));

                //trunc 1st 2 numbers
                rgbval =((string) hexval).substr(2,6);

                //convert string to hex
                stringstream intstrm;
                intstrm << hex << rgbval;

                //stream to int var
                intstrm >> leftasint;

                 difLeft = abs(asint - leftasint);
            }
            if(i < n - 1)
            {
                //convert int to hex
                sprintf(hexval,"%X", eFunc.pixel(i + 1, j));

                //trunc 1st 2 numbers
                rgbval =((string) hexval).substr(2,6);

                //convert string to hex
                stringstream intstrm;
                intstrm << hex << rgbval;

                //stream to int var
                intstrm >> downasint;

                 difDown = abs(asint - downasint);
            }
            if(j < m - 1)
            {
                //convert int to hex
                sprintf(hexval,"%X", eFunc.pixel(i, j + 1));

                //trunc 1st 2 numbers
                rgbval =((string) hexval).substr(2,6);

                //convert string to hex
                stringstream intstrm;
                intstrm << hex << rgbval;

                //stream to int var
                intstrm >> rightasint;

                 difRight = abs(asint - rightasint);
            }
            */

            if(grayscale)
            {
                eFunc.setPixel(i, j, difUp + difDown + difLeft + difRight);
                QColor clr(eFunc.pixel(i, j));
                //eFunc.setPixel(i, j, qRgb(clr.red() * 0.2126, clr.green() * 0.7152, clr.blue() * 0.0722));

                /*
                //convert int to hex
                sprintf(hexval,"%X", eFunc.pixel(i, j));

                //trunc 1st 2 numbers
                rgbval =((string) hexval).substr(2,6);

                //convert string to hex
                stringstream intstrm;
                intstrm << hex << rgbval;

                //stream to int var
                intstrm >> asint;
                */
            }else
            {
                eFunc.setPixel(i, j, difUp + difDown + difLeft + difRight);
            }
        }
    }

    return eFunc;
}

QImage Retargeting::getEFunc()
{
    return eFunc;
}
