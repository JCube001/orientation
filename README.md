# Orientation

A simple Qt application which displays a rotating cube. The cube rotates based
on quaternion values received via some embedded device over a serial
connection. This application is useful in demonstrating the functionality of an
IMU or AHRS system on an embedded device.

## Protocol

The following is the message format to use when communicating from an embedded
device to this application. The numbers represent the byte offsets.

0      1        5   9   13  17     19
+------+--------+---+---+---+------+
| 0x55 | Scalar | X | Y | Z | CRLF |
+------+--------+---+---+---+------+

The value 0x55 is the header of each message. The CRFL ("\r\n") is the footer
of each message. Scalar is the scalar component of the quaternion as a binary
float. The X, Y, and Z values are all binary floats and they make up the
vector part of the quaternion. Note that you may have to reverse the byte
endianness of each float as you send them over serial so they can be
reconstructed correctly in this application.

