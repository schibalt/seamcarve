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
    
    int upComp = 0;
    int downComp = 0;
    int leftComp = 0;
    int rightComp = 0;
    
    int energy;
    
    /*for (int i = 1; i < m - 1; i++)
    {
        for (int j = 1; j < n - 1; j++)*/
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
                
            energy = abs(upComp + downComp + leftComp + rightComp);
            
            if (energyFunction.format() != 3)
                energyFunction.setPixel(i, j, qRgb(energy, energy, energy));
            else
            {
                if (energy > 255)
                    energy = 255;
                    
                if (energy < 0)
                    energy = 0;
                    
                energyFunction.setPixel(i, j, energy);
            }
        }
        
    //cout << "maxenergy = " << maxenergy / (m * n) << endl;
    energySet = true;
    return energyFunction;
}

bool** Retargeting::setVertSeams(int widthDifference)
{
    int m = image.width();
    int n = image.height();
    
    vector<pair<int, int> > lowestEnergies;
    
    for (int i = 0; i < m; i++)
    {
        int energy = vertSeams[i][n - 1];
        
        //cout << "pushing " << energy << ", " << i << " onto stack" << endl;
        
        //push (energy, index)
        lowestEnergies.push_back(pair<int, int> (energy, i));
    }
    
    /*
    for (size_t x = 0; x < highestEnergies.size(); ++x)
        cout << highestEnergies[x].first << ", " << highestEnergies[x].second << endl;
    
    cout << endl;
    */
    
    // using function as comparator
    sort(lowestEnergies.begin(), lowestEnergies.end());
    lowestEnergies.erase(lowestEnergies.begin() + widthDifference, lowestEnergies.end());
    
    ofstream outFile;
    char outputFilename[] = "C:\\Users\\Andrew\\Documents\\lowestEnergies.txt";
    outFile.open(outputFilename, ios::out);
    
    if (!outFile)
    {
        cerr << "Can't open output file " << outputFilename << endl;
        exit(1);
    }
    // print out content:
    //cout << "lowestEnergies contains:" << endl;
    for (size_t x = 0; x < lowestEnergies.size(); ++x)
        //cout << lowestEnergies[x].first << ", " << lowestEnergies[x].second << endl;
        outFile << lowestEnergies[x].first << ", " << lowestEnergies[x].second << endl;
        
    outFile.close();
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
    
    //for each seam
    for (size_t x = 0; x < lowestEnergies.size(); ++x)
    {
        int column = lowestEnergies[x].second;
        
        //carve out one pixel per row
        for (int j = n - 1; j >= 0; j--)
        {
            vertPixelsRemoved[column][j] = true;
            
            //note the seam energies of the upper pixels
            if (j > 0)
            {
                int upLeft = 0;
                //int up = 0;
                int upRight = 0;
                int currentSeamEnergy = lowestEnergies[x].first;
                int currentpixelEnergy = qGray(energyFunction.pixel(column, j));
                
                //up = vertSeams[column][j - 1];
                //if (up == currentSeamEnergy - currentpixelEnergy && !vertPixelsRemoved[column][j - 1])
                //i'm in trouble
                
                if (column > 0)
                {
                    upLeft = vertSeams[column - 1][j - 1];
                    if (upLeft == currentSeamEnergy - currentpixelEnergy && !vertPixelsRemoved[column - 1][j - 1])
                        //carve diagonally upward-leftward
                        column--;
                }
                if (column < m - 1)
                {
                    upRight = vertSeams[column + 1][j - 1];
                    if (upRight == currentSeamEnergy - currentpixelEnergy && !vertPixelsRemoved[column + 1][j - 1])
                        //carve diagonally upward-rightward
                        column++;
                }
            }
        }
    }
    
    char outputFilename2[] = "C:\\Users\\Andrew\\Documents\\vertPixelsRemoved.txt";
    
    outFile.open(outputFilename2, ios::out);
    
    if (!outFile)
    {
        cerr << "Can't open output file " << outputFilename2 << endl;
        exit(1);
    }
    
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < m; i++)
            outFile << vertPixelsRemoved[i][j] << " ";
            
        outFile << endl;
    }
    outFile.close();
    return vertPixelsRemoved;
}

bool** Retargeting::setLatSeams(int heightDifference)
{
    return latPixelsRemoved;
}

void Retargeting::setVerticalSeamTable()
{
    if (!isImageSet())
    {
        cout << "there's no image to retarget" << endl;
        return;
    }
    setEnergy();
    
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
    
    if (!outFile)
    {
        cerr << "Can't open output file " << outputFilename << endl;
        exit(1);
    }
    
    //for (int j = 1; j < n - 1; j++)
    for (int j = 0; j < n; j++)
    {
        //for (int i = 2; i < m - 2; i++)
        
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
            
            int pixelEnergy = qGray(energyFunction.pixel(i, j));
            int seamEnergy = 0;
            
            //if not on the left edge
            if (i > 0)
            {
                //and not on the right edge
                if (i < m - 1)
                    seamEnergy = pixelEnergy + min(upLeft, min(up, upRight));
                //because on the right edge
                else
                    seamEnergy = pixelEnergy + min(up, upLeft);
            }
            //on the left edge
            else
                seamEnergy = pixelEnergy + min(up, upRight);
                
            vertSeams[i][j] = seamEnergy;
            
            outFile << seamEnergy << "(" << pixelEnergy << ")" << " ";
            //cout << seamEnergy << "(" << pixelEnergy << ")" << " ";
        }
        outFile << endl;
        //cout << endl;
    }
    outFile.close();
}

void Retargeting::setLateralSeamTable()
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
    return energyFunction;
}
