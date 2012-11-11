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
#include <algorithm>
#include <fstream>

using namespace std;

Retargeting::Retargeting()
{
    imageSet = false;
}

Retargeting::Retargeting(const Retargeting& orig)
{
}

Retargeting::~Retargeting()
{
}

bool Retargeting::setImage(string path)
{
    imagePath = path;
    QString QsPath;
    cout << "trying to set " << path << " as image for retarget" << endl;
    image.load(QsPath.fromStdString(path));
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
    return image;
}

void Retargeting::setEnergy(QImage energy)
{
    this->energy = energy;
}

QImage Retargeting::energyFunction(QImage localImage)
{
    //there are usually m rows in a matrix but QImage uses cartesian coordinates
    //so there are m columns (x-sub-m)
    int m = localImage.width();
    
    //vice versa with n (n is y-sub-n)
    int n = localImage.height();
    
    cout << "copying.  width = " << localImage.width() << " height = " << localImage.height() << endl;
    QImage localEnergy = localImage.copy(0, 0, localImage.width(), localImage.height());
    
    int upComp = 0;
    int downComp = 0;
    int leftComp = 0;
    int rightComp = 0;
    
    int energyVal;
    
    for (int i = 0; i < m ; i++)
        for (int j = 0; j < n; j++)
        {
            if (i > 0)
                upComp = qGray(localImage.pixel(i, j)) - qGray(localImage.pixel(i - 1, j));
                
            if (i < m - 1)
                downComp = qGray(localImage.pixel(i, j)) - qGray(localImage.pixel(i + 1, j));
                
            if (j > 0)
                leftComp = qGray(localImage.pixel(i, j)) - qGray(localImage.pixel(i, j - 1));
                
            if (j < n - 1)
                rightComp = qGray(localImage.pixel(i, j)) - qGray(localImage.pixel(i , j + 1));
                
            energyVal = abs(upComp + downComp + leftComp + rightComp);
            
            if (energyVal > 255)
                energyVal = 255;
                
            if (energyVal < 0)
                energyVal = 0;
                
            if (localEnergy.format() != 3)
                localEnergy.setPixel(i, j, qRgb(energyVal, energyVal, energyVal));
            else
                localEnergy.setPixel(i, j, energyVal);
        }
        
    return localEnergy;
}

bool Retargeting::carveVertSeams(int widthDifference)
{
    if (!isImageSet())
    {
        cout << "there's no image to retarget" << endl;
        return false;
    }
    setEnergy(energyFunction(image));

    int m = image.width();
    int n = image.height();

    QImage newImage = image.copy(0, 0, m, n);
    QImage oldImage;

    QImage newEnergy = energy.copy(0, 0, m, n);
    QImage oldEnergy;
    
    //for storing cumulative energy matrices
    int** vertSeams;

    //for storing lookup matrices
    int** vertCarvingDirections;
    
    //for storing pixel masks
    bool** vertPixelsRemoved;

    for (int s = 0; s < widthDifference; s++)
    {
        int format = image.format();

        vertSeams = new int*[m];

        for (int i = 0; i < m; ++i)
        {
            vertSeams[i] = new int[n];

            for (int j = 0; j < n; j++)
                vertSeams[i][j] = 0;
        }

        vertCarvingDirections = new int*[m];

        for (int i = 0; i < m; ++i)
        {
            vertCarvingDirections[i] = new int[n];

            for (int j = 0; j < n; j++)
                vertCarvingDirections[i][j] = UP;
        }

        ofstream vertSeamsFile;
        char vertSeamsFilename[] = "C:\\Users\\Andrew\\Documents\\vertseams.txt";

        vertSeamsFile.open(vertSeamsFilename, ios::out);

        if (!vertSeamsFile)
        {
            cerr << "Can't open output file " << vertSeamsFilename << endl;
            exit(EXIT_FAILURE);
        }

        ofstream vertSeamDirectionsFilestream;
        char vertSeamDirectionsFilename[] = "C:\\Users\\Andrew\\Documents\\vertseamdirections.txt";

        vertSeamDirectionsFilestream.open(vertSeamDirectionsFilename, ios::out);

        if (!vertSeamDirectionsFilestream)
        {
            cerr << "Can't open output file " << vertSeamDirectionsFilename << endl;
            exit(EXIT_FAILURE);
        }

        int minEnergyCell;
        int minEnergy = 2147483647;

        int upRight = -1;
        int up = -1;
        int upLeft = -1;

        int pixelEnergy = -1;
        int seamEnergy = -1;
        int direction = UP;

        //build the dynamic seam energy table and the lookup table in a big Oh(n * m) operation
        for (int j = 0; j < n; j++)
        {
            minEnergy = 2147483647;

            for (int i = 0; i < m; i++)
            {
                pixelEnergy = qGray(newEnergy.pixel(i, j));

                if (j == 0)
                {
                    vertSeams[i][j] = pixelEnergy;
                    continue;
                }

                up = vertSeams[i][j - 1];
                direction = UP;

                //left edge case
                if (i == 0)
                {
                    upRight = vertSeams[i + 1][j - 1];
                    seamEnergy = pixelEnergy + min(up, upRight);

                    if (upRight < up)
                        direction = UPRIGHT;
                }
                else
                {
                    //right edge case
                    if (i == m - 1)
                    {
                        upLeft = vertSeams[i - 1][j - 1];
                        seamEnergy = pixelEnergy + min(up, upLeft);

                        if (upLeft < up)
                            //the energy's lower inside the image rather than along the edge
                            direction = UPLEFT;
                    }
                    else
                    {
                        upLeft = vertSeams[i - 1][j - 1];
                        upRight = vertSeams[i + 1][j - 1];

                        if (upRight < up)
                            //upright's at most only the second highest
                            direction = UPRIGHT;

                        //but still might be higher than upleft so the seam won't go in that direction
                        if (upLeft < upRight && upLeft < up)
                            //uplefts the lowest
                            direction = UPLEFT;

                        seamEnergy = pixelEnergy + min(upLeft, min(up, upRight));
                    }
                }

                //error
                if (i < 0 || i >= m)
                    exit(1);

                //for the last row keep track of the lowest energy cell.  this is the first
                //pixel for the seam that will be carved from the image bottom-up
                if (j == n - 1 && seamEnergy < minEnergy)
                {
                    minEnergyCell = i;
                    minEnergy = seamEnergy;
                }

                vertSeams[i][j] = seamEnergy;
                vertCarvingDirections[i][j] = direction;

                vertSeamsFile << seamEnergy << "(" << pixelEnergy << ")" << " ";
                vertSeamDirectionsFilestream << direction << " ";
            }
            vertSeamsFile << endl;
            vertSeamDirectionsFilestream << endl;
        }
        vertSeamsFile.close();
        vertSeamDirectionsFilestream.close();

        //init the pixel mask
        vertPixelsRemoved = new bool*[m];

        for (int i = 0; i < m; ++i)
        {
            //cout << i << " ";
            vertPixelsRemoved[i] = new bool[n];

            for (int j = 0; j < n; j++)
                //initially false; pixel isn't removed
                vertPixelsRemoved[i][j] = false;
        }

        //starting column for least energetic seam is paired with lowest energy value
        int column = minEnergyCell;

        //that pixel's removed beforehand
        vertPixelsRemoved[column][n - 1] = true;

        //carve out one pixel per row starting from the bottom
        for (int j = n - 1; j > 0; j--)
        {
            vertPixelsRemoved[column][j] = true;

            if (vertCarvingDirections[column][j - 1] == UPLEFT)
                column--;

            if (vertCarvingDirections[column][j - 1] == UPRIGHT)
                column++;
        }
        vertPixelsRemoved[column][0] = true;

        //init pixel mask file
        ofstream vertPixelsRemovedFile;
        char vertPixelsRemovedFilename[] = "C:\\Users\\Andrew\\Documents\\vertPixelsRemoved.txt";
        vertPixelsRemovedFile.open(vertPixelsRemovedFilename, ios::out);
        vertPixelsRemovedFile << "s:" << s << endl;

        //pixel mask file can't be opened
        if (!vertPixelsRemovedFile)
        {
            cerr << "Can't open output file " << vertPixelsRemovedFilename << endl;
            exit(1);
        }

        //init pixel mask file
        ofstream vertPixelsSetFilestream;
        char vertPixelsSetFilename[] = "C:\\Users\\Andrew\\Documents\\vertPixelsSet.txt";
        vertPixelsSetFilestream.open(vertPixelsSetFilename, ios::out);
        vertPixelsSetFilestream << "s:" << s << endl;

        //pixel mask file can't be opened
        if (!vertPixelsSetFilestream)
        {
            cerr << "Can't open output file " << vertPixelsSetFilename << endl;
            exit(EXIT_FAILURE);
        }

        //oldImage = QImage(m, n, newImage.format());
        //oldEnergy = QImage(m, n, newEnergy.format());

        //save the images before the seam's carved
        oldImage = newImage.copy(0, 0, m, n);
        oldEnergy = newEnergy.copy(0, 0, m, n);

        //init the empty images so the pixels from the old images can be written into them
        //(minus the seam that is being carved out per the pixel mask)
        newImage = QImage(m - 1, n, oldImage.format());
        newEnergy = QImage(m - 1, n, oldEnergy.format());

        if (newImage.format() == 3)
        {
            newEnergy.setNumColors(oldImage.numColors());
            newImage.setNumColors(oldImage.numColors());
            newEnergy.setColorTable(oldImage.colorTable());
            newImage.setColorTable(oldImage.colorTable());
        }

        for (int j = 0; j < n; j++)
        {
            bool seamFound = false;

            for (int i = 0; i < m; i++)
            {
                vertPixelsRemovedFile << vertPixelsRemoved[i][j] << " ";

                if (vertPixelsRemoved[i][j])
                {
                    seamFound = true;
                    continue;
                }

                //if the seam's already been carved out the pixels will have been
                //shifted leftward in the new image
                if (seamFound)
                {
                    int pixelValue = oldImage.pixel(i, j);

                    if (newImage.format() == 3)
                    {
                        pixelValue = qGray(pixelValue);

                        if (pixelValue > 255)
                            pixelValue = 255;

                        if (pixelValue < 0)
                            pixelValue = 0;
                    }
                    vertPixelsSetFilestream << pixelValue << " ";
                    newImage.setPixel(i - 1, j, pixelValue);
                }
                else
                {
                    int pixelValue = oldImage.pixel(i, j);

                    if (oldImage.format() == 3)
                    {
                        pixelValue = qGray(pixelValue);

                        if (pixelValue > 255)
                            pixelValue = 255;

                        if (pixelValue < 0)
                            pixelValue = 0;
                    }

                    vertPixelsSetFilestream << pixelValue << " ";
                    newImage.setPixel(i, j, pixelValue);
                    //localEnergy.setPixel(i, j, energyVal);}
                    //localEnergy.setPixel(i, j, qRgb(energyVal, energyVal, energyVal));
                }
            } //row done

            vertPixelsRemovedFile << "j:" << j << endl;
            vertPixelsSetFilestream << "j:" << j << endl;
        }//seam done
        vertPixelsRemovedFile.close();
        vertPixelsSetFilestream.close();
        newEnergy = energyFunction(newImage);
        m--;
    }//all seams done
    retargetedImage = newImage;
    retargetSuccess = true;
    return retargetSuccess;
}

QImage Retargeting::getRetargetedImage()
{
    return retargetedImage;
}

bool Retargeting::isRetargetSuccessful()
{
    return retargetSuccess;
}

void Retargeting::setVerticalSeamTable(int m, int n)
{
}

void Retargeting::carveLatSeams(int heightDifference)
{
}

void Retargeting::setLateralSeamTable(int m, int n)
{
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
    return energy;
}
