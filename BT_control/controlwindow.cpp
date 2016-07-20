#include <QDebug>
#include <QGridLayout>
#include <QMessageBox>

#include "controlwindow.h"

ControlWindow::ControlWindow(QWidget *parent)
:   QMainWindow(parent)
{
    nameLineEdit = new QLineEdit();
    connectStatusLineEdit = new QLineEdit();
    nameLineEdit->setReadOnly(true);
    connectStatusLineEdit->setReadOnly(true);

    connectBtn = new QPushButton();
    connectBtn->setText("Connect");

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLineEdit, 0, 0);
    mainLayout->addWidget(connectStatusLineEdit, 0, 1);
    mainLayout->addWidget(connectBtn, 1, 0, 1, 2);

    QWidget *window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);
    setWindowTitle(tr("Bluetooth Control"));

    connect(connectBtn, &QPushButton::clicked,
            this, &ControlWindow::on_connectBtnClicked);
}

ControlWindow::~ControlWindow()
{
    qDebug() << "ControlWindow destructor";
    delete btSocket;
}

void ControlWindow::on_connectBtnClicked(bool)
{
    if (QBluetoothLocalDevice().hostMode() == QBluetoothLocalDevice::HostPoweredOff) {
        QMessageBox::information(this, "Bluetooth power",
                                 "Check Bluetooth is powered on");
        return;
    }

    connectBtn->setEnabled(false);
    QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
    DeviceBTSelect deviceSelect;
    deviceSelect.startScan();
    if ( deviceSelect.exec() == QDialog::Accepted ) {
        QBluetoothDeviceInfo deviceInfo = deviceSelect.deviceInfo();
        nameLineEdit->setText(deviceInfo.name());

        btSocket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
        qDebug() << "Create socket";
        btSocket->connectToService(deviceInfo.address(),
                                   deviceInfo.deviceUuid());
                                   //QBluetoothUuid(serviceUuid));
        qDebug() << "ConnectToService done";

        connect(btSocket, &QBluetoothSocket::connected,
                this, &ControlWindow::btSocketConnected);

        connect(btSocket, &QBluetoothSocket::disconnected,
                this, &ControlWindow::btSocketDisconnected);

        connect(btSocket, static_cast<void(QBluetoothSocket::*)(QBluetoothSocket::SocketError)>(&QBluetoothSocket::error),
                this, &ControlWindow::btSocketError);
    }

    connectBtn->setEnabled(true);
}

void ControlWindow::btSocketDisconnected()
{
    connectStatusLineEdit->setText("Disconnected from" + btSocket->peerName());
}

void ControlWindow::btSocketConnected()
{
    connectStatusLineEdit->setText("Connected to " + btSocket->peerName());
}

void ControlWindow::btSocketError(QBluetoothSocket::SocketError error)
{
    connectStatusLineEdit->setText("Error: " + QString::number(error));
    qDebug() << connectStatusLineEdit->text();
}

void ControlWindow::keyPressEvent(QKeyEvent *event)
{
    if (   !btSocket
        || !btSocket->isOpen()) {
        return;
    }

    char keyInput = event->key();
    qDebug() << keyInput;
    if (    keyInput == 'W'
        ||  keyInput == 'S'
        ||  keyInput == 'A'
        ||  keyInput == 'D'
        ||  keyInput == ' ') {
        btSocket->write(&keyInput, 1);
    } else {
        QWidget::keyPressEvent(event);
    }
}
