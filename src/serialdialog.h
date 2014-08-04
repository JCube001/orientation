#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H

#include <QDialog>
#include <QSerialPort>

namespace Ui {
class SerialDialog;
}

class SerialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialDialog(QWidget *parent = 0);
    ~SerialDialog();
    QString portName() const;
    QSerialPort::BaudRate baudRate() const;
    QSerialPort::DataBits dataBits() const;
    QSerialPort::StopBits stopBits() const;
    QSerialPort::Parity parity() const;
    QSerialPort::FlowControl flowControl() const;

private slots:
    void showSerialPortInfo(int index);
    void refreshPorts();

private:
    Ui::SerialDialog *ui;
};

#endif // SERIALDIALOG_H
