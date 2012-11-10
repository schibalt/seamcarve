/*
 * File:   retargeting.h
 * Author: Andrew Tilisky
 *
 * Created on November 2, 2012, 10:41 PM
 */

#ifndef RETARGETING_H
#define RETARGETING_H
#include <string>
#include <QGraphicsPixmapItem>
using namespace std;

class Retargeting
{
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
        void setVerticalSeamTable();
        void setLateralSeamTable();
        bool isEnergySet();
        bool isImageSet();
        bool** setVertSeams(int widthDifference);
        bool** setLatSeams(int heightDifference);
    private:
        std::string imagePath;
        QImage image;
        struct sort_pred;
        QImage energyFunction;
        int** vertSeams;
        int** latSeams;
        bool energySet;
        bool imageSet;
        bool** vertPixelsRemoved;
        bool** latPixelsRemoved;
};

/*
template < class T1, class T2, class Pred = std::less<T> >
struct sort_pair_second
{
    bool operator()(const std::pair<T1, T2>&left, const std::pair<T1, T2>&right)
    {
        Pred p;
        return p(left.first, right.first);
    }
};
*/

#endif  /* RETARGETING_H */

