#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QByteArray>
#include <QMessageBox>

#include "glwidget.h"
#include "serialdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serial(new QSerialPort(this)),
    protocol(new Protocol),
    protocolThread(new QThread(this))
{
    ui->setupUi(this);

    // Connect actions
    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(protocol, SIGNAL(rotationReady(QQuaternion)), ui->glWidget, SLOT(setRotation(QQuaternion)));

    // Setup worker threads
    protocol->moveToThread(protocolThread);
    protocolThread->start();
}

MainWindow::~MainWindow()
{
    protocolThread->quit();
    protocolThread->wait();
    delete ui;
}

void MainWindow::openSerialPort()
{
    SerialDialog dialog(this);
    if (dialog.exec() == SerialDialog::Accepted)
    {
        QString portName = dialog.portName();
        serial->setPortName(portName);
        serial->setBaudRate(dialog.baudRate());
        serial->setDataBits(dialog.dataBits());
        serial->setStopBits(dialog.stopBits());
        serial->setParity(dialog.parity());
        serial->setFlowControl(dialog.flowControl());
        if (serial->open(QIODevice::ReadOnly))
        {
            ui->actionOpen->setEnabled(false);
            ui->actionClose->setEnabled(true);
            ui->statusBar->showMessage(tr("Opened ") + portName);
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), serial->errorString());
            ui->statusBar->showMessage(tr("Open error"));
        }
    }
}

void MainWindow::closeSerialPort()
{
    serial->close();
    ui->actionOpen->setEnabled(true);
    ui->actionClose->setEnabled(false);
    ui->statusBar->showMessage(tr("Closed port"));
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    for (const char &ch : data)
    {
        protocol->parse(ch);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About"),
                       tr("A tool to visualize the orientation computed by an embedded motion tracking device."));
}
