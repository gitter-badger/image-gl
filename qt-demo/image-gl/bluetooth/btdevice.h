#ifndef BTDEVICE_H
#define BTDEVICE_H

#include <QObject>
#include <QBluetoothAddress>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>

class QLowEnergyController;
class BtDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name )
    Q_PROPERTY(QBluetoothAddress address READ address)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)

public:

    enum State {
        NotConnected = 0,
        Connected = 1,
        ScanningServices = 2,
        ScanningDone = 3,
        Unknown = 255
    };

    BtDevice( const QBluetoothDeviceInfo &info, QObject *parent = 0);
    ~BtDevice();

    QString name() const
    {
        return m_name;
    }

    QBluetoothAddress address() const
    {
        return m_address;
    }

    void connectDevice();
    void disconnectDevice();

    State state() const
    {
        return m_state;
    }

protected slots:
    void setState(State arg)
    {
        if (m_state == arg)
            return;

        m_state = arg;
        emit stateChanged(arg);
    }

signals:
    void stateChanged(State arg);

private slots:
    void deviceConnected();
    void errorReceived(QLowEnergyController::Error);
    void deviceDisconnected();
    void addLowEnergyService(QBluetoothUuid);
    void serviceScanDone();

private:
    QString m_name;
    QBluetoothAddress m_address;
    QLowEnergyController *m_lowEnergyController;

    State m_state;
};

#endif // BTDEVICE_H
