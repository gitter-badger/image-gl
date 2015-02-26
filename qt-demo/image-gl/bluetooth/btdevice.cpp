#include "btdevice.h"
//#include <QLowEnergyController>

BtDevice::BtDevice(const QBluetoothDeviceInfo &info, QObject *parent):
    QObject(parent),
    m_state(BtDevice::NotConnected),
    m_lowEnergyController(NULL)
{
    info.address();
    info.name();
}

BtDevice::~BtDevice()
{

}

void BtDevice::connectDevice(){

//    if (!m_lowEnergyController) {
//        // Connecting signals and slots for connecting to LE services.
//        m_lowEnergyController = new QLowEnergyController(address());
//        connect(m_lowEnergyController, SIGNAL(connected()),
//                this, SLOT(deviceConnected()));
//        connect(m_lowEnergyController, SIGNAL(error(QLowEnergyController::Error)),
//                this, SLOT(errorReceived(QLowEnergyController::Error)));
//        connect(m_lowEnergyController, SIGNAL(disconnected()),
//                this, SLOT(deviceDisconnected()));
//        connect(m_lowEnergyController, SIGNAL(serviceDiscovered(QBluetoothUuid)),
//                this, SLOT(addLowEnergyService(QBluetoothUuid)));
//        connect(m_lowEnergyController, SIGNAL(discoveryFinished()),
//                this, SLOT(serviceScanDone()));
//    }

//    /// TODO:
////    if (isRandomAddress())
//        m_lowEnergyController->setRemoteAddressType(QLowEnergyController::RandomAddress);
////    else
////        m_lowEnergyController->setRemoteAddressType(QLowEnergyController::PublicAddress);
//    m_lowEnergyController->connectToDevice();
}

void BtDevice::disconnectDevice(){
//    if(m_lowEnergyController){
//        m_lowEnergyController->disconnectFromDevice();
//    }
}

void BtDevice::deviceConnected()
{
    setState(BtDevice::Connected);
}

//void BtDevice::errorReceived(QLowEnergyController::Error error)
//{

//}

void BtDevice::deviceDisconnected()
{
    setState(BtDevice::NotConnected);
}

//void BtDevice::addLowEnergyService(QBluetoothUuid uuid)
//{

//}

void BtDevice::serviceScanDone()
{
    setState(BtDevice::ScanningDone);
}
