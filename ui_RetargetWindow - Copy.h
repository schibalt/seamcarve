/********************************************************************************
** Form generated from reading UI file 'RetargetWindow.ui'
**
** Created: Sat Nov 3 18:24:26 2012
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RETARGETWINDOW_H
#define UI_RETARGETWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RetargetWindow
{
public:
    QAction *actionNew;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QProgressBar *progressBar;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RetargetWindow)
    {
        if (RetargetWindow->objectName().isEmpty())
            RetargetWindow->setObjectName(QString::fromUtf8("RetargetWindow"));
        RetargetWindow->resize(411, 425);
        actionNew = new QAction(RetargetWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        centralwidget = new QWidget(RetargetWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        gridLayout->addWidget(progressBar, 1, 0, 1, 1);

        RetargetWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RetargetWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 411, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        RetargetWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RetargetWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        RetargetWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNew);

        retranslateUi(RetargetWindow);

        QMetaObject::connectSlotsByName(RetargetWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RetargetWindow)
    {
        RetargetWindow->setWindowTitle(QApplication::translate("RetargetWindow", "RetargetWindow", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("RetargetWindow", "New", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("RetargetWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RetargetWindow: public Ui_RetargetWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETARGETWINDOW_H
