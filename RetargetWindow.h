/* 
 * File:   RetargetWindow.h
 * Author: Andrew Tilisky
 *
 * Created on November 2, 2012, 1:54 PM
 */

#ifndef _RETARGETWINDOW_H
#define	_RETARGETWINDOW_H

#include "Retargeting.h"
#include "ui_RetargetWindow.h"
#include <QProgressBar>

class RetargetWindow : public QMainWindow {
    Q_OBJECT
public:
    RetargetWindow();
    virtual ~RetargetWindow();
private:
    Ui::RetargetWindow widget;
    Retargeting retarget;
    bool showImage();
    bool showImage(QImage image);
    QGraphicsScene *scene;
    void writePix(QImage image);
    //QProgressBar *progressBar;
private slots:
    void on_actionNew_triggered();
    void on_retargetButton_clicked();
    void on_showEFuncButton_clicked();
    void on_showImageButton_clicked();
};

#endif	/* _RETARGETWINDOW_H */

