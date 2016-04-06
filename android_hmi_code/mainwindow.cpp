#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_2->setText("<font color='Black'>Process value");
    ui->label_3->setText("<font color='Black'>Set value");
    connect(ui->Valve_command,SIGNAL(pressed()),this,SLOT(btn_valve()));                // Connect Valve push button signal to slot
    connect(ui->horizontalSlider,SIGNAL(sliderReleased()),this,SLOT(slider_setvalue()));//Connect slider signal to slot
    x=new usb();                                            // Object of class Usb
    x->init_usb();                                          // Initilize ttyGS0
    chk_btn_valve=0;                                        // Status flag for valve QPushButton
}

MainWindow::~MainWindow()           // Destructor of this class
{
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

void MainWindow::update()
{
    quint16 ptr[4]={0};         // Array to be transmitted
/*    1st element = Function code
 *    2nd element = Register address
 *    3rd element = No. of Register NIitem
 *    4th element = Data (Register Value)
*/
    /********** Recieves Data for QlineEdit i.e Process Value ************/
    ptr[0]=3;ptr[1]=0x2;ptr[2]=1;ptr[3]=0;              // This is a Holding register for Process Value tag.
    x->comm((char*)ptr,&(ptr[3]));                      // Send the request and responce is recieved in ptr[3] which holds the data
    ui->Process_Value->setText(QString::number(ptr[3]));// Display the value on QlineEdit

    /********** Recieves Data for QSlider i.e Set Value ************/

    ptr[0]=3;ptr[1]=0x1;ptr[2]=1;ptr[3]=0;      // This is a Holding register for Set Value (Horizontal Slider) tag.
        x->comm((char*)ptr,&(ptr[3]));
    ui->horizontalSlider->setValue(ptr[3]);     // Display value on Horizontal slider

    /********** Recieves Data for QPushButton i.e Motor Status ************/

    ptr[0]=1;ptr[1]=0x1;ptr[2]=1;ptr[3]=0;      // This is a Coil register with address 1
    x->comm((char*)ptr,&(ptr[3]));
    if(ptr[3] == 1) ui->Motor_status->setStyleSheet(" background-color: rgb(100,200,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");//Changes the colour of button according to its on/off status
    else ui->Motor_status->setStyleSheet("background-color: rgb(255,150,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");

    /********** Recieves Data for QPushButton i.e Valve Status ************/

    ptr[0]=1;ptr[1]=0x2;ptr[2]=1;ptr[3]=0;      // This is a Coil register with address 2
    x->comm((char*)ptr,&(ptr[3]));
    if(ptr[3] == 1) ui->Valve_command->setStyleSheet(" background-color: rgb(100,200,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");
    else ui->Valve_command->setStyleSheet("background-color: rgb(255,150,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");
}

void MainWindow::btn_valve()        // This function is called when button is clicked
{
    quint16 ptr[4]={0};         // Array to be transmitted
/*    1st element = Function code
 *    2nd element = Register address
 *    3rd element = No. of Register NIitem
 *    4th element = Data (Register Value)
*/
    if(chk_btn_valve == 0)
    {
        chk_btn_valve=1;
        qDebug("btn_valve==0\n");
    }
    else
    {
        chk_btn_valve=0;
        qDebug("btn_valve==1\n");
    }

    ptr[0]=5;ptr[1]=0x2;ptr[2]=1;ptr[3]=chk_btn_valve;      // Function code 5: To write value to coil
    x->comm((char*)ptr,&(ptr[3]));

    ptr[0]=1;ptr[1]=0x2;ptr[2]=1;ptr[3]=0;                  // Read the value of the coil and change the button appearence accordingly
    x->comm((char*)ptr,&(ptr[3]));
    if(ptr[3] == 1) ui->Valve_command->setStyleSheet(" background-color: rgb(100,200,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");
    else ui->Valve_command->setStyleSheet("background-color: rgb(255,150,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");
}

void  MainWindow::slider_setvalue()     //This function is called when slider is released after sliding
{
        quint16 ptr[4]={0};         // Array to be transmitted
    /*    1st element = Function code
     *    2nd element = Register address
     *    3rd element = No. of Register NIitem
     *    4th element = Data (Register Value)
    */
    ptr[0]=6;ptr[1]=0x1;ptr[2]=1;ptr[3]=ui->horizontalSlider->value();      // Function code 6: To write value to Holding register
    x->comm((char*)ptr,&(ptr[3]));
}
