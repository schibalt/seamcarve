/********************************************************************************
** Form generated from reading UI file 'RetargetWindow.ui'
**
** Created: Tue Nov 13 11:15:13 2012
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
#include <QtGui/QPushButton>
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
    QPushButton *retargetButton;
    QPushButton *showEFuncButton;
    QPushButton *showImageButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RetargetWindow)
    {
        if (RetargetWindow->objectName().isEmpty())
            RetargetWindow->setObjectName(QString::fromUtf8("RetargetWindow"));
        RetargetWindow->resize(658, 653);
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
        progressBar->setEnabled(false);
        progressBar->setValue(0);

        gridLayout->addWidget(progressBar, 7, 0, 1, 1);

        retargetButton = new QPushButton(centralwidget);
        retargetButton->setObjectName(QString::fromUtf8("retargetButton"));
        retargetButton->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(retargetButton, 3, 0, 1, 1);

        showEFuncButton = new QPushButton(centralwidget);
        showEFuncButton->setObjectName(QString::fromUtf8("showEFuncButton"));
        showEFuncButton->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(showEFuncButton, 2, 0, 1, 1);

        showImageButton = new QPushButton(centralwidget);
        showImageButton->setObjectName(QString::fromUtf8("showImageButton"));
        showImageButton->setMaximumSize(QSize(80, 16777215));

        gridLayout->addWidget(showImageButton, 1, 0, 1, 1);

        RetargetWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RetargetWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 658, 21));
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
        retargetButton->setText(QApplication::translate("RetargetWindow", "Retarget", 0, QApplication::UnicodeUTF8));
        showEFuncButton->setText(QApplication::translate("RetargetWindow", "Show energy function", 0, QApplication::UnicodeUTF8));
        showImageButton->setText(QApplication::translate("RetargetWindow", "Show image", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("RetargetWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RetargetWindow: public Ui_RetargetWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETARGETWINDOW_H
