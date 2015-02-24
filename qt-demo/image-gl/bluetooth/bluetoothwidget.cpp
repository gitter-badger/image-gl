#include "bluetoothwidget.h"
#include "ui_bluetoothwidget.h"

#include "btdevice.h"

BluetoothWidget::BluetoothWidget(QWidget *parent) :
    QWidget(parent),
    m_isBluetoothOn( false ),
    m_discoveryAgent( NULL ),
    m_lowEnergyController( NULL ),
    ui(new Ui::BluetoothWidget)
{
    ui->setupUi(this);
}

BluetoothWidget::~BluetoothWidget()
{
    delete ui;
}



void BluetoothWidget::scanBluetooth(){
    if(m_discoveryAgent && !m_discoveryAgent->isActive()){
        m_discoveryAgent->start();
    }
}

void BluetoothWidget::initBluetooth()
{
    if(!m_isBluetoothOn){
        m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

        bool ok = false;
        ok = connect(m_discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
                     this, SLOT(onAddDevice(const QBluetoothDeviceInfo&)));                      Q_ASSERT(ok);

        ok = connect(m_discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
                     this, SLOT(onDeviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));      Q_ASSERT(ok);

        ok = connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(onDeviceScanFinished()));  Q_ASSERT(ok);

        m_discoveryAgent->start();
        m_isBluetoothOn = true;
    }
}

void BluetoothWidget::onAddDevice(const QBluetoothDeviceInfo& info){
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        BtDevice *d = new BtDevice(info);
        m_discoveredDevices.append(d);
    }
}

void BluetoothWidget::onDeviceScanError(QBluetoothDeviceDiscoveryAgent::Error error){
    /// TODO: LOG ERROR
}

void BluetoothWidget::onDeviceScanFinished(){

}



void BluetoothWidget::on_pushButtonBTConnectSelected_clicked()
{
//    if (!m_lowEnergyController) {
//        // Connecting signals and slots for connecting to LE services.
//        m_lowEnergyController = new QLowEnergyController(currentDevice.getDevice().address());
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
