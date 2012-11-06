********************************************************************************
** Form generated from reading UI file 'RetargeterWindow.ui'
**
** Created: Thu Nov 1 21:05:31 2012
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RETARGETERWINDOW_H
#define UI_RETARGETERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RetargeterWindow
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RetargeterWindow)
    {
        if (RetargeterWindow->objectName().isEmpty())
            RetargeterWindow->setObjectName(QString::fromUtf8("RetargeterWindow"));
        RetargeterWindow->resize(800, 600);
        centralwidget = new QWidget(RetargeterWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        RetargeterWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RetargeterWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        RetargeterWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RetargeterWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        RetargeterWindow->setStatusBar(statusbar);

        retranslateUi(RetargeterWindow);

        QMetaObject::connectSlotsByName(RetargeterWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RetargeterWindow)
    {
        RetargeterWindow->setWindowTitle(QApplication::translate("RetargeterWindow", "RetargeterWindow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RetargeterWindow: public Ui_RetargeterWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETARGETERWINDOW_H
