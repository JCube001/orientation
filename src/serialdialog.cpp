#include "serialdialog.h"
#include "ui_serialdialog.h"

#include <QSerialPortInfo>
#include <QString>
#include <QStringList>

SerialDialog::SerialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialDialog)
{
    ui->setupUi(this);

    // Connect actions
    connect(ui->portComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showSerialPortInfo(int)));
    connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(refreshPorts()));

    // Populate devices
    refreshPorts();

    // Populate baud rates
    for (const qint32 &baud : QSerialPortInfo::standardBaudRates())
    {
        ui->baudRateComboBox->addItem(QString::number(baud), baud);
    }
    ui->baudRateComboBox->setCurrentText(QStringLiteral("9600"));

    // Populate data bits
    ui->dataBitsComboBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsComboBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsComboBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsComboBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsComboBox->setCurrentIndex(3);

    // Populate stop bits
    ui->stopBitsComboBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->stopBitsComboBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
    ui->stopBitsComboBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    // Populate parity
    ui->parityComboBox->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    ui->parityComboBox->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    ui->parityComboBox->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    ui->parityComboBox->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);
    ui->parityComboBox->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);

    // Populate flow control
    ui->flowControlComboBox->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    ui->flowControlComboBox->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlComboBox->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);
}

SerialDialog::~SerialDialog()
{
    delete ui;
}

QString SerialDialog::portName() const
{
    return ui->portComboBox->currentText();
}

QSerialPort::BaudRate SerialDialog::baudRate() const
{
    return static_cast<QSerialPort::BaudRate>(ui->baudRateComboBox->currentData().toInt());
}

QSerialPort::DataBits SerialDialog::dataBits() const
{
    return static_cast<QSerialPort::DataBits>(ui->dataBitsComboBox->currentData().toInt());
}

QSerialPort::StopBits SerialDialog::stopBits() const
{
    return static_cast<QSerialPort::StopBits>(ui->stopBitsComboBox->currentData().toInt());
}

QSerialPort::Parity SerialDialog::parity() const
{
    return static_cast<QSerialPort::Parity>(ui->parityComboBox->currentData().toInt());
}

QSerialPort::FlowControl SerialDialog::flowControl() const
{
    return static_cast<QSerialPort::FlowControl>(ui->flowControlComboBox->currentData().toInt());
}

void SerialDialog::showSerialPortInfo(int index)
{
    if (Q_LIKELY(index != -1))
    {
        QStringList list = ui->portComboBox->itemData(index).toStringList();
        ui->descriptionLabel->setText(list.at(1));
        ui->manufacturerLabel->setText(list.at(2));
        ui->serialNumberLabel->setText(list.at(3));
        ui->locationLabel->setText(list.at(4));
        ui->vendorIdLabel->setText(list.at(5));
        ui->productIdLabel->setText(list.at(6));
    }
}

void SerialDialog::refreshPorts()
{
    static const QString blank = tr("N/A");
    ui->portComboBox->clear();
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
    {
        QString description = info.description();
        QString manufacturer = info.manufacturer();
        QString serialNumber = info.serialNumber();
        QStringList list;
        list << info.portName()
             << (!description.isEmpty() ? description : blank)
             << (!manufacturer.isEmpty() ? manufacturer : blank)
             << (!serialNumber.isEmpty() ? serialNumber : blank)
             << info.systemLocation()
             << (info.hasVendorIdentifier() ? QStringLiteral("0x") + QString::number(info.vendorIdentifier(), 16) : blank)
             << (info.hasProductIdentifier() ? QStringLiteral("0x") + QString::number(info.productIdentifier(), 16) : blank);
        ui->portComboBox->addItem(list.first(), list);
    }
}
