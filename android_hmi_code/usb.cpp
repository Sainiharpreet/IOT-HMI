#include "usb.h"
#include"ctime"

usb::usb(QObject *parent) :
    QObject(parent)
{
}

inline void mydelay(clock_t milli_sec) // clock_t is a like typedef unsigned int clock_t. Use clock_t instead of integer in this context
{
clock_t start_time = clock();
clock_t end_time = milli_sec  + start_time;
while(clock() != end_time);
}

void usb::comm(char *p, quint16 *i)             // Function for sending modbus request and recieving modbus response
{
    int temp =0;char *t="k";
    qDebug("before file write");
    temp = write(fd,p,7);                       // Write 7 bytes of modbus request to usb serial port
    qDebug("after file write \n");
    if(temp == -1)qDebug("error while writing");
    temp =0;
    qDebug("before file read");
    while(temp < 2)                             // Logic for reading exactly 2 bytes of modbus response
    {
        temp = read(fd,(char*)i,2);//(char*)i
        qDebug("temp = %d",temp);
    }
     write(fd,t,1);                             // Tx acknowledgment character
    qDebug("no. of bytes read= %d",temp);
}

int usb::init_usb()                             // Initialize and open Usb serial Port and returns the file descriptor
{
    portname = "/dev/ttyGS0";
    fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);// opens the i/o file and returns the fle descriptor
    if (fd < 0)
    {
          qDebug( "error %d opening %s: %s", errno, portname, strerror (errno));//prints the error if any in debug mode
            return -1 ;
    }

    set_interface_attribs (fd, B9600, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking (fd, 0);                // set no blocking
    qDebug("usb file descriptor open\n");
    return fd;
}

int usb::set_interface_attribs (int fd, int speed, int parity)  // Sets the attributes and returns 0 on success and -1 on failure
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                qDebug("\nerror from tcgetattr\n");
                return -1;
        }

        cfsetospeed (&tty, speed);                      // set o/p speed
        cfsetispeed (&tty, speed);                      // set i/p speed

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
                tty.c_cflag |= parity;
                tty.c_cflag &= ~CSTOPB;
                tty.c_cflag &= ~CRTSCTS;

                if (tcsetattr (fd, TCSANOW, &tty) != 0)
                {
                        qDebug("\nerror from tcsetaatr\n");
                        return -1;
                }
                return 0;
        }

void usb::set_blocking (int fd, int should_block)// sets blocking or nonblocking i/o operation
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                qDebug("error %d from tggetattr\n", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0) qDebug("error %d setting term attributes\n", errno);
}
