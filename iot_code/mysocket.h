#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/QTcpSocket>

class mysocket:public QObject
{
    Q_OBJECT
public:
   explicit  mysocket(QObject *parent = 0);
    bool doconnect();                   // Connects to embedded socket
    ~mysocket();
    void comm(char *p, quint16 *i);     // Send request and recieves response
    void createsocket();                // Creates tcp socket
    QTcpSocket *socket;                 // Pointer to QTcpSocket class
signals:

public slots:

private:

};

#endif // MYSOCKET_H
