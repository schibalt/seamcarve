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
    /*
    //delete &imagePath;
    cout << "imagepath deleted" << endl;
    //delete &energyFunction;
    cout << "energyFunction deleted" << endl;
    
    // De-Allocate memory to prevent memory leak
    for (int i = 0; i < image.width(); ++i)
        delete [] vertSeams[i];
    
    cout << "vertSeams 2nd dim arrays deleted" << endl;
    delete [] vertSeams;
    cout << "vertSeams deleted" << endl;
    
    // De-Allocate memory to prevent memory leak
    //for (int i = 0; i < image.height(); ++i)
    //delete [] latSeams[i];
    
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

QImage Retargeting::energyFunction(QImage image)
{
    //there are usually m rows in a matrix but QImage uses cartesian coordinates
    //so there are m columns (x-sub-m)
    int m = image.width();
    
    //vice versa with n (n is y-sub-n)
    int n = image.height();
    
    cout << "copying.  width = " << image.width() << " height = " << image.height() << endl;
    energy = image.copy(0, 0, image.width(), image.height());
    
    int upComp = 0;
    int downComp = 0;
    int leftComp = 0;
    int rightComp = 0;
    
    int energyVal;

    for (int i = 0; i < m ; i++)
        for (int j = 0; j < n; j++)
        {
            if (i > 0)
                upComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i - 1, j));
                
            if (i < m - 1)
                downComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i + 1, j));
                
            if (j > 0)
                leftComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i, j - 1));
                
            if (j < n - 1)
                rightComp = qGray(image.pixel(i, j)) - qGray(image.pixel(i , j + 1));
                
            energyVal = abs(upComp + downComp + leftComp + rightComp);
            
            if (energyVal > 255)
                energyVal = 255;
                
            if (energyVal < 0)
                energyVal = 0;
                
            if (energy.format() != 3)
                energy.setPixel(i, j, qRgb(energyVal, energyVal, energyVal));
            else
                energy.setPixel(i, j, energyVal);
        }

    return energy;
}

void Retargeting::carveVertSeams(int widthDifference)
{
    if (!isImageSet())
    {
        cout << "there's no image to retarget" << endl;
        return;
    }
    setEnergy(energyFunction(image));

    int m = image.width();
    int n = image.height();

    //return cumulative energy matrices
    int** vertSeams;

    //return cumulative energy matrices
    int** vertCarvingDirections;

    //pixel masks
    bool** vertPixelsRemoved;

    int** imageMatrix;
    int** energyMatrix;

    for(int s = 0; s < widthDifference; s++)
    {
        imageMatrix = new int*[m];

        for (int i = 0; i < m; ++i)
        {
            imageMatrix[i] = new int[n];

            for (int j = 0; j < n; j++){
                if(s == 0)
                imageMatrix[i][j] = image.pixel(i, j);
                else{
                    imageMatrix[i][j] = image.pixel(i, j);

                }
            }
        }

        energyMatrix = new int*[m];

        for (int i = 0; i < m; ++i)
        {
            energyMatrix[i] = new int[n];

            for (int j = 0; j < n; j++)
                energyMatrix[i][j] = energy.pixel(i, j);
        }

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

        ofstream outFile;
        char outputFilename[] = "C:\\Users\\Andrew\\Documents\\vertseams.txt";

        outFile.open(outputFilename, ios::out);

        if (!outFile)
        {
            cerr << "Can't open output file " << outputFilename << endl;
            exit(1);
        }

        ofstream outFile2;
        char outputFilename2[] = "C:\\Users\\Andrew\\Documents\\vertseamdirections.txt";

        outFile2.open(outputFilename2, ios::out);

        if (!outFile2)
        {
            cerr << "Can't open output file " << outputFilename2 << endl;
            exit(1);
        }

        for (int j = 0; j < n; j++)
        {
            for (int i = 0; i < m; i++)
            {
                int upRight = 0;
                int up = 0;
                int upLeft = 0;

                if (j > 0)
                {
                    up = vertSeams[i][j - 1];

                    if (i < m - 1)
                        upRight = vertSeams[i + 1][j - 1];

                    if (i > 0)
                        upLeft = vertSeams[i - 1][j - 1];
                }

                int pixelEnergy = qGray(energy.pixel(i, j));
                int seamEnergy = 0;
                int direction = UP;

                //inner case
                if (i > 0 && i < m - 1)
                {
                    //and not on the right edge
                    if (upRight < up)
                        direction = UPRIGHT;

                    if (upLeft < upRight && upLeft < up)
                        direction = UPLEFT;

                    seamEnergy = pixelEnergy + min(upLeft, min(up, upRight));
                }

                //right edge case
                if (i >= m - 1)
                {
                    seamEnergy = pixelEnergy + min(up, upLeft);

                    if (upLeft < up)
                        direction = UPLEFT;
                }

                //left edge case
                if (i == 0)
                {
                    seamEnergy = pixelEnergy + min(up, upRight);

                    if (upRight < up)
                        direction = UPRIGHT;
                }

                //error
                if (i < 0 || i >= m)
                    exit(1);

                vertSeams[i][j] = seamEnergy;
                vertCarvingDirections[i][j] = direction;

                outFile << seamEnergy << "(" << pixelEnergy << ")" << " ";
                outFile2 << direction << " ";
            }
            outFile << endl;
            outFile2 << endl;
        }
        outFile.close();
        outFile2.close();

        vector<pair<int, int> > lowestEnergies;

        for (int i = 0; i < m; i++)
        {
            int energy = vertSeams[i][n - 1];

            //push (energy, index)
            lowestEnergies.push_back(pair<int, int> (energy, i));
        }

        // using function as comparator
        sort(lowestEnergies.begin(), lowestEnergies.end());
        lowestEnergies.erase(lowestEnergies.begin() + widthDifference, lowestEnergies.end());

        ofstream outFile3;
        char outputFilename3[] = "C:\\Users\\Andrew\\Documents\\lowestEnergies.txt";
        outFile3.open(outputFilename3, ios::out);

        if (!outFile3)
        {
            cerr << "Can't open output file " << outputFilename3 << endl;
            exit(1);
        }
        // print out content:
        for (size_t x = 0; x < lowestEnergies.size(); ++x)
            outFile3 << lowestEnergies[x].first << ", " << lowestEnergies[x].second << endl;

        outFile3.close();
        /*
        if(widthDifference == highestEnergies.size())
            cout << "if the vector of highest energies is sorted correctly then the seams can be carved" << endl;
        else
            cout << "the vector of highest energies doesn't have a starting pixel for each seam" << endl;
        */

        // Allocate memory
        vertPixelsRemoved = new bool*[m];

        for (int i = 0; i < m; ++i)
        {
            //cout << i << " ";
            vertPixelsRemoved[i] = new bool[n];

            for (int j = 0; j < n; j++)
                vertPixelsRemoved[i][j] = false;
        }

        int column = lowestEnergies[0].second;

        //carve out one pixel per row
        for (int j = n - 1; j >= 0; j--)
        {
            vertPixelsRemoved[column][j] = true;

            //decide how to modify the column
        }

        ofstream outFile4;
        char outputFilename4[] = "C:\\Users\\Andrew\\Documents\\vertPixelsRemoved.txt";
        outFile4.open(outputFilename4, ios::out);

        if (!outFile4)
        {
            cerr << "Can't open output file " << outputFilename4 << endl;
            exit(1);
        }

        for (int j = 0; j < n; j++)
        {
            for (int i = 0; i < m; i++)
                outFile4 << vertPixelsRemoved[i][j] << " ";

            outFile4 << endl;
        }
        outFile4.close();
    }
}

void Retargeting::setVerticalSeamTable(int m, int n)
{
}

void Retargeting::carveLatSeams(int heightDifference)
{
    //int** latSeams;
    //int** latCarvingDirections;
    //bool** latPixelsRemoved;
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
