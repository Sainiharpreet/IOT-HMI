#include <QApplication>
#include "mysocket.h"
#include <widget.h>
#include "ui_widget.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QTimer *timer = new QTimer();       // Object of pointer to QTimer class
    QApplication a(argc, argv);         //Object of class QApplication which manages the gui control flow
    Widget w;                           // object of class widget to manage our gui components
    w.show();                           // GUI becomes visible
    QObject::connect(timer,SIGNAL(timeout()),&w,SLOT(update())); // Connect the timeout signal to update slot
    timer->start(9000);                 // Timer gives timeout after 9 sec.
    return a.exec();
}
