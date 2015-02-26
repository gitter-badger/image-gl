#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QHash>

#include <QAbstractSocket>

class QByteArray;
class QThread;
class QTcpServer;
class QTcpSocket;
class listener : public QObject
{
    Q_OBJECT
public:
    explicit listener(QObject *parent = 0);
    ~listener();

signals:
    void sensorData(qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal,bool,bool);
    void dataReceived(QByteArray);

public slots:
    void connectionAvailable();

private slots:
    void readyRead();
    void socketDisconnect();
    void socketError(QAbstractSocket::SocketError error);
    void socketStateChanged(QAbstractSocket::SocketState state);

private:
    QTcpServer *m_tcp;
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely

};

#endif // LISTENER_H
