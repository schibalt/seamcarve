/*
 * File:   main.cpp
 * Author: Andrew Tilisky
 *
 * Created on October 31, 2012, 9:52 PM
 */

#include <QtGui/QApplication>
#include "RetargetWindow.h"
#include <qdebug.h>

int main(int argc, char *argv[])
{
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    // create and show your widgets here
    RetargetWindow mainWindow ;
    mainWindow.show();
    return app.exec();
}

