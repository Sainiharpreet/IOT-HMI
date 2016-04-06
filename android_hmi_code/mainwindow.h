#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"usb.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();
    Ui::MainWindow *ui;
    usb *x;                     // Pointer to usb class
private:
//    Ui::MainWindow *ui;
    quint16 chk_btn_valve;      // Status flag for valve push button
private slots:
    void update();              // This slot is called periodically. check main function for signal slot connection
    void btn_valve();           // This slot is called when QPushbutton is clicked. Check constructor of this class for signal slot connection
    void slider_setvalue();     // This slot is called slider is released after sliding. Check constructor of this class for signal slot connection
};

#endif // MAINWINDOW_H
