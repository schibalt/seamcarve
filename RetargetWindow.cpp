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

    int viewWidth = widget.graphicsView->width();
    int viewHeight = widget.graphicsView->height();

    cout << "view is " << viewWidth << ", " << viewHeight;

    bool retargetSuccess = retarget.isRetargetSuccessful();

    int imageWidth = retarget.getImage().width();
    int imageHeight = retarget.getImage().height();

    QImage retargetedImage;
    int retargetedWidth;
    int retargetedHeight;

    int widthDiff;
    int heightDiff;

    if (retargetSuccess)
    {
        retargetedImage = retarget.getRetargetedImage();
        retargetedWidth = retargetedImage.width();
        retargetedHeight = retargetedImage.height();

        cout << " and retargeted image is " << retargetedWidth << ", " << retargetedHeight << endl;
        widthDiff = viewWidth - retargetedWidth;
        heightDiff = viewHeight - retargetedHeight;
    }
    else
    {
        cout << " and image is " << imageWidth << ", " << imageHeight << endl;
        widthDiff = viewWidth - imageWidth;
        heightDiff = viewHeight - imageHeight;
    }

    bool viewTooSkinny = false;

    if (widthDiff < 0)
        viewTooSkinny = true;

    bool viewTooShort = false;

    if (heightDiff < 0)
        viewTooShort = true;

    //status bar info
    if (!viewTooSkinny && !viewTooShort)
    {
        widget.statusbar->showMessage("The image doesn't need to be retargeted");
        return;
    }
    else
    {
        stringstream ss;
        ss << "Retargeting " << imageWidth << ", " << imageHeight << " image to ";

        if (viewTooSkinny)
            ss << viewWidth;
        else
            ss << imageWidth;

        ss << ", ";

        if (viewTooShort)
            ss << viewHeight;
        else
            ss << imageHeight;

        widget.statusbar->showMessage(QString::fromStdString(ss.str()));
    }
    int offset = 2;

    if (viewTooSkinny)
        retargetSuccess = retarget.carveVertSeams(abs(widthDiff) + offset);

    if (retargetSuccess)
    {
        retargetedImage = retarget.getRetargetedImage();

        retargetedWidth = retargetedImage.width();
        retargetedHeight = retargetedImage.height();

        cout << "retargeted image is " << retargetedWidth << ", " << retargetedHeight;
        cout << " after vertical seam removal" << endl;
    }
    else
        exit(EXIT_FAILURE);

    if (viewTooShort)
        retargetSuccess = retarget.carveLatSeams(abs(heightDiff) + offset);

    if (retargetSuccess)
    {
        retargetedImage = retarget.getRetargetedImage();

        retargetedWidth = retargetedImage.width();
        retargetedHeight = retargetedImage.height();

        cout << "retargeted image is " << retargetedWidth << ", " << retargetedHeight;
        cout << " after lateral seam removal" << endl;
    }
    else
        exit(EXIT_FAILURE);

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
