#ifndef BLUETOOTHWIDGET_H
#define BLUETOOTHWIDGET_H

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QWidget>

namespace Ui {
class BluetoothWidget;
}

class BtDevice;
class BluetoothWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BluetoothWidget(QWidget *parent = 0);
    ~BluetoothWidget();

    void scanBluetooth();
    void initBluetooth();

public slots:
    void on_pushButtonBTConnectSelected_clicked();
    void onAddDevice(const QBluetoothDeviceInfo&);
    void onDeviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void onDeviceScanFinished();

private:

    bool m_isBluetoothOn;
    QBluetoothDeviceDiscoveryAgent *  m_discoveryAgent;
    QLowEnergyController *m_lowEnergyController;
    QList<BtDevice *> m_discoveredDevices;


    Ui::BluetoothWidget *ui;
};

#endif // BLUETOOTHWIDGET_H
