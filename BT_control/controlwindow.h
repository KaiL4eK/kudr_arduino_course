#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QKeyEvent>
#include <QBluetoothSocket>

#include "devicebtselect.h"

namespace Ui {
class ControlWindow;
}

class ControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControlWindow(QWidget *parent = 0);
    ~ControlWindow();
private slots:
    void on_connectBtnClicked(bool);
    void btSocketConnected();
    void btSocketError(QBluetoothSocket::SocketError);
private:
    QLineEdit *nameLineEdit;
    QLineEdit *connectStatusLineEdit;
    QPushButton *connectBtn;
    QBluetoothSocket *btSocket;

    void keyPressEvent(QKeyEvent *event);
};

#endif // CONTROLWINDOW_H
