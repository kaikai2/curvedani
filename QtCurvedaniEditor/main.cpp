#include <QApplication>
#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
