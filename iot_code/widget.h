#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mysocket.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
     Ui::Widget *ui;
    mysocket *x;                        // pointer to class mysocket
public slots:


private slots:
    void update();                     // This slot is called periodically. check main function for signal slot connection
    void btn_valve();                  // This slot is called when QPushbutton is clicked. Check constructor of this class for signal slot connection
    void slider_setvalue();            // This slot is called slider is released after sliding. Check constructor of this class for signal slot connection

private:
//    Ui::Widget *ui;
    quint16 chk_btn_valve=0;        // Status flag for valve push button

};

#endif // WIDGET_H
