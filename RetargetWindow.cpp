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

    /*
    connect(this->widget.actionNew, SIGNAL(triggered(bool)), this, SLOT(actionNew_Triggered()));
    connect(this->widget.showImageButton, SIGNAL(clicked()), this, SLOT(eFuncButton_clicked()));
    connect(this->widget.showEFuncButton, SIGNAL(clicked()), this, SLOT(showButton_clicked()));
    */

    //progressBar = new QProgressBar();
    //cout<<progressBar->maximum()<<endl;
    //widget.statusbar->addWidget( progressBar, 1 );
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

    /*
    char hexval [8];
    string rgbval;
    int asint;

    for (float i = 0; i < width; i++)
    {
        for (int j = 0; j < image.height(); j++)
        {
            //convert int to hex
            sprintf(hexval,"%X", image.pixel(i, j));

            //trunc 1st 2 numbers
            rgbval =((string) hexval).substr(2,6);

            //convert string to hex
            stringstream intstrm;
            intstrm << hex << rgbval;

            //stream to int var
            intstrm >> asint;

            //cout << asint <<" (" << rgbval <<") ";
        }
        widget.progressBar->setValue((i / width)*100);
        //cout << "progress = " << (i / width)*100 << endl;
        //cout << endl;
    }
    */
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
    retarget.setEnergy();
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

    if (widget.graphicsView->width() < retarget.getImage().width())
    {
        viewTooSkinny = true;
        cout << "making vert seam matrix" << endl;
        retarget.setVerticalSeamTable();
    }
    /*
    if (widget.graphicsView->height() < retarget.getImage().height())
    {
        viewTooShort = true;
        cout << "making lat seam matrix" << endl;
        retarget.getLateralSeamTable();
    }
    */

    //status bar info
    if (!viewTooShort && !viewTooSkinny)
        widget.statusbar->showMessage("The view isn't small enough to retarget the image");
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
    }

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
        //cout << fileExtension.toStdString() << endl;
        isMatch = fileExtensionList.contains(fileExtension.toUpper());
    }

    widget.statusbar->showMessage(filename);

    if (!isMatch)
        widget.statusbar->showMessage("this type of file isn't supported");
    else
    {
        if (retarget.setImagePath(filename.toStdString()))
            widget.statusbar->showMessage("imagepath set to " + filename);
        else
            widget.statusbar->showMessage("imagepath set failure");

        if (retarget.setImage(filename.toStdString()))
            widget.statusbar->showMessage("image set to " + filename);
        else
            widget.statusbar->showMessage("imagepath set failure");

        showImage(retarget.getImage());
    }
}
