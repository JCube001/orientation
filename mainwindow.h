#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QThread>

#include "protocol.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void readData();
    void about();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    Protocol *protocol;
    QThread *protocolThread;
};

#endif // MAINWINDOW_H
