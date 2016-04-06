#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include"qdebug.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    x=new mysocket();             // Create object of class mysocket
    x->createsocket();            // Create Tcp/ip Socket
    connect(ui->Valve_command,SIGNAL(pressed()),this,SLOT(btn_valve()));                // Connect Valve push button signal to slot
    connect(ui->horizontalSlider,SIGNAL(sliderReleased()),this,SLOT(slider_setvalue()));//Connect slider signal to slot
}

Widget::~Widget()       // Destructor of this class
{
    delete ui;
}

void Widget::update()
{
    quint16 ptr[4]={0};         // Array to be transmitted
/*    1st element = Function code
 *    2nd element = Register address
 *    3rd element = No. of Register NIitem
 *    4th element = Data (Register Value)
*/

    /********** Recieves Data for QlineEdit i.e Process Value ************/

    ptr[0]=3;ptr[1]=0x2;ptr[2]=1;ptr[3]=0;      // This is a Holding register for Process Value tag.
    if(x->doconnect())                          // Establish connection with server
    {
        x->comm((char*)ptr,&(ptr[3]));          // Send the request and responce is recieved in ptr[3] which holds the data
    ui->Process_Value->setText(QString::number(ptr[3]));// Display the value on QlineEdit
    }

    /********** Recieves Data for QSlider i.e Set Value ************/

    ptr[0]=3;ptr[1]=0x1;ptr[2]=1;ptr[3]=0;      // This is a Holding register for Set Value (Horizontal Slider) tag.
    if(x->doconnect())
    {
        x->comm((char*)ptr,&(ptr[3]));
    ui->horizontalSlider->setValue(ptr[3]);     // Display value on Horizontal slider
    }

    /********** Recieves Data for QPushButton i.e Motor Status ************/

    ptr[0]=1;ptr[1]=0x1;ptr[2]=1;ptr[3]=0;      // This is a Coil register with address 1
    if( x->doconnect())
    {
        x->comm((char*)ptr,&(ptr[3]));
    if(ptr[3] == 1) ui->Motor_status->setStyleSheet(" background-color: rgb(100,200,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");//Changes the colour of button according to its on/off status
    else ui->Motor_status->setStyleSheet("background-color: rgb(255,150,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");
    }

    /********** Recieves Data for QPushButton i.e Valve Status ************/

    ptr[0]=1;ptr[1]=0x2;ptr[2]=1;ptr[3]=0;      // This is a Coil register with address 2
    if( x->doconnect())
    {
        x->comm((char*)ptr,&(ptr[3]));
    if(ptr[3] == 1) ui->Valve_command->setStyleSheet(" background-color: rgb(100,200,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");
    else ui->Valve_command->setStyleSheet("background-color: rgb(255,150,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");
    }
}

void Widget::btn_valve()        // This function is called when button is clicked
{
    quint16 ptr[4]={0};         // Array to be transmitted
/*    1st element = Function code
 *    2nd element = Register address
 *    3rd element = No. of Register NIitem
 *    4th element = Data (Register Value)
*/
    if(chk_btn_valve == 0)      // Status flag of the Valve
    {
        chk_btn_valve=1;        // Change the status to on if it's previous state was off
        qDebug("btn_valve==0\n");
    }
    else
    {
        chk_btn_valve=0;        // Change the status to off if it's previous state was on
        qDebug("btn_valve==1\n");
    }

    ptr[0]=5;ptr[1]=0x2;ptr[2]=1;ptr[3]=chk_btn_valve; // Function code 5: To write value to coil
    if( x->doconnect()) x->comm((char*)ptr,&(ptr[3]));

    ptr[0]=1;ptr[1]=0x2;ptr[2]=1;ptr[3]=0;              // Read the value of the coil and change the button appearence accordingly
    if( x->doconnect())
    {
        x->comm((char*)ptr,&(ptr[3]));
    if(ptr[3] == 1) ui->Valve_command->setStyleSheet(" background-color: rgb(100,200,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");
    else ui->Valve_command->setStyleSheet("background-color: rgb(255,150,100) ;border-width: 0px;border-color: #339;border-style: solid;border-radius: 7;");
    }
}
void  Widget::slider_setvalue()     //This function is called when slider is released after sliding
{
    quint16 ptr[4]={0};         // Array to be transmitted
/*    1st element = Function code
 *    2nd element = Register address
 *    3rd element = No. of Register NIitem
 *    4th element = Data (Register Value)
*/
    ptr[0]=6;ptr[1]=0x1;ptr[2]=1;ptr[3]=ui->horizontalSlider->value();    // Function code 6: To write value to Holding register
    if( x->doconnect()) x->comm((char*)ptr,&(ptr[3]));
}
