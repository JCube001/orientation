#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QByteArray>
#include <QObject>
#include <QQuaternion>

class Protocol : public QObject
{
    Q_OBJECT

public:
    explicit Protocol(QObject *parent = 0);
    void parse(const char ch);

signals:
    void rotationReady(QQuaternion);
    void error();

private:
    enum class State
    {
        Header,
        Data,
        Footer
    };

    QQuaternion quaternion;
    QByteArray buffer;
    QString footer;
    State state;
};

#endif // PROTOCOL_H
