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
    QImage setEFunc();
    QImage Sobel();
    QImage getEFunc();
    void verticalSeams();
    void horizontalSeams();
private:
    std::string imagePath;
    QImage image;
    QImage eFunc;
    double **vertSeams;
    double **latSeams;

};

#endif	/* RETARGETING_H */

