/*
 * File:   RetargetWindow.cpp
 * Author: Andrew Tilisky
 *
 * Created on November 2, 2012, 1:54 PM
 */

#include "RetargetWindow.h"
#include <QFileDialog>
#include <QRegExp>
#include <iostream>
#include <QProgressBar>
#include <sstream>

using namespace std;

RetargetWindow::RetargetWindow()
{
    widget.setupUi(this);
    scene = new QGraphicsScene(this);
    widget.graphicsView->setScene(scene);
}

RetargetWindow::~RetargetWindow()
{
}

bool RetargetWindow::showImage(QImage image)
{
    // the items (lines and points) in the graphicsview
    QList<QGraphicsItem*> list = (*scene).items();

    // delete everything.  this is a new case.
    QList<QGraphicsItem*>::Iterator it = list.begin();
    for (; it != list.end(); ++it)
    {
        if (*it)
        {
            (*scene).removeItem(*it);
            delete *it;
        }
    }
    QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    (*scene).addItem(Qgpmi);
    widget.graphicsView->setSceneRect(QRect(0, 0, image.width(), image.height()));
    QString imgPath;
    stringstream ss;
    ss << (int) image.format();

    widget.statusbar->showMessage(imgPath.fromStdString(retarget.getImagePath() + " Format: " + ss.str()));
    writePix(image);
    return true;
}

bool RetargetWindow::showImage()
{
    // the items (lines and points) in the graphicsview
    QList<QGraphicsItem*> list = (*scene).items();

    // delete everything.  this is a new case.
    QList<QGraphicsItem*>::Iterator it = list.begin();
    for (; it != list.end(); ++it)
    {
        if (*it)
        {
            (*scene).removeItem(*it);
            delete *it;
        }
    }
    QImage image = retarget.getImage();
    QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    (*scene).addItem(Qgpmi);
    widget.graphicsView->setSceneRect(QRect(0, 0, image.width(), image.height()));
    QString imgPath;
    stringstream ss;
    ss << (int) image.format();

    widget.statusbar->showMessage(imgPath.fromStdString(retarget.getImagePath() + " Format: " + ss.str()));
    writePix(image);
    return true;
}

void RetargetWindow::writePix(QImage image)
{
    int width = image.width();
    vector<string> colorlist;
    cout << "image width = " << width << endl;
    widget.progressBar->setTextVisible(true);

    widget.progressBar->setValue(100);
}

void RetargetWindow::on_showImageButton_clicked()
{
    if (!retarget.isImageSet())
    {
        widget.statusbar->showMessage("Go to File > New and select an image");
        return;
    }
    showImage();
}

void RetargetWindow::on_showEFuncButton_clicked()
{
    if (!retarget.isImageSet())
    {
        widget.statusbar->showMessage("Go to File > New and select an image");
        return;
    }
    retarget.setEnergy(retarget.energyFunction(retarget.getImage()));

    // the items (lines and points) in the graphicsview
    QList<QGraphicsItem*> list = (*scene).items();

    // delete everything.  this is a new case.
    QList<QGraphicsItem*>::Iterator it = list.begin();
    for (; it != list.end(); ++it)
    {
        if (*it)
        {
            (*scene).removeItem(*it);
            delete *it;
        }
    }
    QImage image = retarget.getEnergy();
    QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    (*scene).addItem(Qgpmi);
    widget.graphicsView->setSceneRect(QRect(0, 0, image.width(), image.height()));

    QString imgPath;
    stringstream ss;
    ss << (int) image.format();

    widget.statusbar->showMessage(imgPath.fromStdString(retarget.getImagePath() + " Format: " + ss.str()));
}

void RetargetWindow::on_retargetButton_clicked()
{
    if (!retarget.isImageSet())
    {
        widget.statusbar->showMessage("Go to File > New and select an image");
        return;
    }

    bool viewTooShort = false;
    bool viewTooSkinny = false;

    cout << "view is " << widget.graphicsView->width() << ", " << widget.graphicsView->height();
    cout << " and image is " << retarget.getImage().width() << ", " << retarget.getImage().height() << endl;

    int widthDiff = widget.graphicsView->width() - retarget.getImage().width();

    if (widthDiff < 0)
        viewTooSkinny = true;

    int heightDiff = widget.graphicsView->height() - retarget.getImage().height();

    if (heightDiff < 0)
        viewTooShort = true;

    QImage retargetedImage;
    bool retargetSuccess = false;
    int offset = 2;

    //status bar info
    if ( !viewTooSkinny)
        widget.statusbar->showMessage("The view isn't skinny enough to retarget the image");
    else
    {
        stringstream ss;
        ss << "Retargeting " << retarget.getImage().width() << ", " << retarget.getImage().height() << " image to ";

        if (viewTooSkinny)
            ss << widget.graphicsView->width();
        else
            ss << retarget.getImage().width();

        ss << ", ";

        if (viewTooShort)
            ss << widget.graphicsView->height();
        else
            ss << retarget.getImage().height();

        widget.statusbar->showMessage(QString::fromStdString(ss.str()));

        if (viewTooSkinny)
            retargetSuccess = retarget.carveVertSeams(abs(widthDiff) + offset);
    }

    // the items (lines and points) in the graphicsview
    QList<QGraphicsItem*> list = (*scene).items();

    // delete everything.  this is a new case.
    QList<QGraphicsItem*>::Iterator it = list.begin();
    for (; it != list.end(); ++it)
    {
        if (*it)
        {
            (*scene).removeItem(*it);
            delete *it;
        }
    }

    QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(retargetedImage));
    (*scene).addItem(Qgpmi);
    widget.graphicsView->setSceneRect(QRect(0, 0, retargetedImage.width(), retargetedImage.height()));

     heightDiff = widget.graphicsView->height() - retarget.getImage().height();

    if (heightDiff < 0)
        viewTooShort = true;

    //status bar info
    if ( !viewTooShort)
        widget.statusbar->showMessage("The view isn't short enough to retarget the image");
    else
    {
        stringstream ss;
        ss << "Retargeting " << retarget.getImage().width() << ", " << retarget.getImage().height() << " image to ";

        if (viewTooSkinny)
            ss << widget.graphicsView->width();
        else
            ss << retarget.getImage().width();

        ss << ", ";

        if (viewTooShort)
            ss << widget.graphicsView->height();
        else
            ss << retarget.getImage().height();

        widget.statusbar->showMessage(QString::fromStdString(ss.str()));

        if (viewTooShort)
            retargetSuccess = retarget.carveLatSeams(abs(heightDiff) + offset);
    }

    if (retargetSuccess)
        retargetedImage = retarget.getRetargetedImage();
    else
        return;

    // the items (lines and points) in the graphicsview
     list = (*scene).items();

    // delete everything.  this is a new case.
     it = list.begin();
    for (; it != list.end(); ++it)
    {
        if (*it)
        {
            (*scene).removeItem(*it);
            delete *it;
        }
    }

     Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(retargetedImage));
    (*scene).addItem(Qgpmi);
    widget.graphicsView->setSceneRect(QRect(0, 0, retargetedImage.width(), retargetedImage.height()));
}

void RetargetWindow::on_actionNew_triggered()
{
    widget.statusbar->showMessage("selecting new file");
    QString filename;
    bool isMatch = false;
    QStringList fileExtensionList;

    filename = QFileDialog::getOpenFileName(this, tr("Find Files"), QDir::currentPath());
    fileExtensionList << "BMP" << "GIF" << "JPG" << "JPEG" << "PNG" << "PBM"
                      << "PGM" << "PPM" << "XBM" << "XPM";

    QStringList splitFilenameList = filename.split(".");
    if (splitFilenameList.size() > 1)
    {
        QString fileExtension = splitFilenameList[splitFilenameList.size() - 1];

        fileExtension = fileExtension.toUpper();
        isMatch = fileExtensionList.contains(fileExtension);
    }

    widget.statusbar->showMessage(filename);

    if (isMatch)
    {
        string svFilename = filename.toStdString();

        if (retarget.setImage(svFilename))
            widget.statusbar->showMessage("image set to " + filename);
        else
            widget.statusbar->showMessage("imagepath set failure");

        showImage(retarget.getImage());
    }
    else
        widget.statusbar->showMessage("this type of file isn't supported");
}
