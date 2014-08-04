#include "protocol.h"

Protocol::Protocol(QObject *parent) :
    QObject(parent),
    state(State::Header)
{
}

/**
 * @brief Protocol::parse
 * @param ch
 *
 * The message format. The numbers represent the byte offsets.
 *
 * 0      1        5   9   13  17     19
 * +------+--------+---+---+---+------+
 * | 0x55 | Scalar | X | Y | Z | CRLF |
 * +------+--------+---+---+---+------+
 */
void Protocol::parse(const char ch)
{
    static const int MAGIC_HEADER = 0x55;
    static const int SIZE_OF_DATA = sizeof(float) * 4;
    static const int SIZE_OF_FOOTER = 2;
    switch (state)
    {
    case State::Header:
        if (ch == MAGIC_HEADER)
        {
            buffer.clear();
            footer.clear();
            state = State::Data;
        }
        break;
    case State::Data:
        buffer.append(ch);
        if (buffer.size() == SIZE_OF_DATA)
        {
            state = State::Footer;
        }
        break;
    case State::Footer:
        footer += ch;
        if (footer.size() == SIZE_OF_FOOTER)
        {
            if (footer == QStringLiteral("\r\n"))
            {
                const float *array = reinterpret_cast<float *>(buffer.data());
                quaternion.setScalar(array[0]);
                quaternion.setX(array[1]);
                quaternion.setY(array[2]);
                quaternion.setZ(array[3]);
                emit rotationReady(quaternion);
            }
            else
            {
                emit error();
            }
            state = State::Header;
        }
        break;
    default:
        break;
    }
}
