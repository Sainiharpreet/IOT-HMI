#ifndef USB_H
#define USB_H

#include <QObject>
#include"qdebug.h"
#include"termios.h"
#include"unistd.h"
#include"errno.h"
#include"sys/types.h"
#include"sys/stat.h"
#include"fcntl.h"

class usb : public QObject
{
    Q_OBJECT
public:
    int fd;                             // file descriptor for usb serial port
    char *portname ;                    // i/o file name eg:- /dev/ttyGS0
    explicit usb(QObject *parent = 0);
    void comm(char *p, quint16 *i);     // Function for sending modbus request and recieving modbus response
    int init_usb();                     // Initialize and open Usb serial Port and returns the file descriptor
    int set_interface_attribs (int fd, int speed, int parity);// Sets the attributes and returns 0 on success and -1 on failure
    void set_blocking (int fd, int should_block);// sets blocking or nonblocking i/o operation
signals:
    
public slots:
    
};

#endif // USB_H
