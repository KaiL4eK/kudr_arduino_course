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
}

void ControlWindow::on_connectBtnClicked(bool)
{
    if (QBluetoothLocalDevice().hostMode() == QBluetoothLocalDevice::HostPoweredOff) {
        QMessageBox::information(this, "Bluetooth power",
                                 "Check Bluetooth is powered on");
        return;
    }

    connectBtn->setEnabled(false);

    DeviceBTSelect deviceSelect;
    deviceSelect.startScan();
    if ( deviceSelect.exec() == QDialog::Accepted ) {
        QBluetoothDeviceInfo deviceInfo = deviceSelect.deviceInfo();
        nameLineEdit->setText(deviceInfo.name());

        btSocket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
        qDebug() << "Create socket";
        btSocket->connectToService(deviceInfo.address(),
                                   deviceInfo.deviceUuid());
        qDebug() << "ConnectToService done";

        connect(btSocket, &QBluetoothSocket::connected,
                this, &ControlWindow::btSocketConnected);

        connect(btSocket, &QBluetoothSocket::error,
                this, &ControlWindow::btSocketError);
    }

    connectBtn->setEnabled(true);
}

void ControlWindow::btSocketConnected()
{
    connectStatusLineEdit->setText("Connected to " + btSocket->peerName());
}

void ControlWindow::btSocketError(QBluetoothSocket::SocketError error)
{
    connectStatusLineEdit->setText("Error: ");
}

void ControlWindow::keyPressEvent(QKeyEvent *event)
{
    int keyInput = event->key();
    switch (keyInput) {
        case 'w':
        case 'a':
        case 's':
        case 'd':
        case ' ':
            qDebug() << "!";
        default:
            QWidget::keyPressEvent(event);
    }
}
