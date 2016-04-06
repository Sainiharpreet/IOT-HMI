#include "mysocket.h"
#include <QtNetwork/QTcpSocket>
#include <QDebug>

mysocket::mysocket(QObject *parent) : QObject(parent)
{

}
void mysocket::createsocket()       // Creates tcp socket
{
    socket = new QTcpSocket(this);
    qDebug("Socket created..\n");
}

bool mysocket ::doconnect()         // Connects to the embedded server
{

    char i=0;
    i = socket->state();//check socket state
    if(i == 0)                                          // state = unconnected
    {
        socket->connectToHost("169.254.134.39",1010);

        if(socket->waitForConnected(3000))              // Wait for 3 sec for connection to establish
        {
            qDebug() << "Connected!";
            return true;
        }
        else
        {
            qDebug() << "Not connected!";
            return false;
        }
    }
    else
    {
        i=socket->error();
        qDebug("Socket state: %d",i);
        qDebug() << socket->errorString();              // print the socket error in debug mode
        if(i == 3) return true;                         // state = connected
        else return false;
    }
}

mysocket::~mysocket()
{
    // close the connection
    socket->close();
}

void mysocket::comm(char *p,quint16 *i)
{
    // send
    qDebug("array of structur = %d %d %d %d %d %d %d %d\n",*p,*(p+1),*(p+2),*(p+3),*(p+4),*(p+5),*(p+6),*(p+7));
    socket->write(p,7);                   // write the request of 7 bytes to socket
    socket->waitForBytesWritten(1000);
    socket->waitForReadyRead(3000);
    while(socket->bytesAvailable()<2);
    socket->read((char*)i,2);             // Read the response of 2 bytes
    socket->close();                      // Close the socket
}
