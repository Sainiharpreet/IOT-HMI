#include <mainwindow.h>
#include <QApplication>
#include"qdebug.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
    mainWindow.showFullScreen();
    QTimer *timer = new QTimer();           // Object of pointer to QTimer class
    QObject::connect(timer,SIGNAL(timeout()),&mainWindow,SLOT(update()));// Connect the timeout signal to update slot
    timer->start(9000);                     // Timer gives timeout after 9 sec.
    return app.exec();

}
