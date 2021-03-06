#include "listener.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

listener::listener(QObject *parent) : QObject(parent)
{
    m_tcp = new QTcpServer(this);

    m_tcp->listen(QHostAddress::AnyIPv4, 8080);
    connect(m_tcp, SIGNAL(newConnection()), SLOT(connectionAvailable()));
}

listener::~listener()
{

}

void listener::connectionAvailable(){
    QTcpSocket *socket = m_tcp->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), SLOT(socketDisconnect()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(socketStateChanged(QAbstractSocket::SocketState)));
}

void listener::socketDisconnect(){
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->disconnectFromHost();
    socket->deleteLater();
}

void listener::socketError(QAbstractSocket::SocketError error)
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    qDebug() << __FUNCTION__ << error;
    socket->disconnectFromHost();
}

void listener::socketStateChanged(QAbstractSocket::SocketState state)
{

}

void listener::readyRead(){
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());

    QByteArray bytes;
    while(socket->bytesAvailable() > 0){
        bytes.append( socket->readAll() );
    }
    QList<QByteArray> lines = bytes.split('\n');

    if(lines.count() > 0){
        QByteArray firstline = lines.first();

        QList<QByteArray> parts =  firstline.split('/');
        QByteArray data = (parts.at(1));
        QList<QByteArray> list1 =  data.split(' ');
        QList<QByteArray> list2 = list1.first().split(',');


        qreal timestamp, ax, ay, az, gx, gy, gz, rx, ry, rz;

        timestamp = ax = ay = az = gx = gy = gz = rx = ry = rz = 0;

        if(list2.count() >= 7){
            timestamp = list2.at(0).toFloat();
            ax        = list2.at(1).toFloat();
            ay        = list2.at(2).toFloat();
            az        = list2.at(3).toFloat();
            gx        = list2.at(4).toFloat();
            gy        = list2.at(5).toFloat();
            gz        = list2.at(6).toFloat();
        }

        bool b1 = false;
        bool b2 = false;

        if(list2.count() >= 9){
            b1 = list2.at(7).toUInt() != 0;
            b2 = list2.at(8).toUInt() != 0;
        }

        emit sensorData( timestamp, ax, ay, az, gx, gy, gz, rx, ry, rz, b1, b2 );
    }

    socket->disconnectFromHost();
}

