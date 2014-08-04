INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    src/serialdialog.ui \
    src/mainwindow.ui

HEADERS += \
    src/protocol.h \
    src/serialdialog.h \
    src/mainwindow.h \
    src/glwidget.h

SOURCES += \
    src/protocol.cpp \
    src/serialdialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/glwidget.cpp

RESOURCES += \
    src/shaders.qrc

