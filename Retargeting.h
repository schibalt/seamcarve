/* 
 * File:   retargeting.h
 * Author: Andrew Tilisky
 *
 * Created on November 2, 2012, 10:41 PM
 */

#ifndef RETARGETING_H
#define	RETARGETING_H
#include <string>
#include <QGraphicsPixmapItem>

class Retargeting {
public:
    Retargeting();
    Retargeting(const Retargeting& orig);
    virtual ~Retargeting();
    bool setImagePath(std::string path);
    std::string getImagePath();
    bool setImage();
    bool setImage(std::string path);
    QImage getImage();
    QImage setEnergy();
    QImage getEnergy();
    void getVerticalSeamTable();
    void getLateralSeamTable();
    bool isEnergySet();
    bool isImageSet();
    bool** setVertSeams(int widthDifference);
    bool** setLatSeams(int heightDifference);
private:
    std::string imagePath;
    QImage image;
    QImage energyFunction;
    int **vertSeams;
    int **latSeams;
    bool energySet;
    bool imageSet;
    bool **vertPixelsRemoved;
    bool **latPixelsRemoved;
};

#endif	/* RETARGETING_H */

