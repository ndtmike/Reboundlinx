QT += widgets serialport

TARGET = reboundlinx
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    parser.cpp \
    datapacket.cpp \
    reboundlinxsplash.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    parser.h \
    datapacket.h \
    reboundlinxsplash.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    reboundlinxsplash.ui

RESOURCES += \
    terminal.qrc


DISTFILES +=

win32{
RC_ICONS += images/NDT_reb4.ico
VERSION = 3.0.0.0
QMAKE_TARGET_COMPANY = James Instruments Inc.
QMAKE_TARGET_PRODUCT = Reboundlinx
QMAKE_TARGET_DESCRIPTION = Upload Software for James Instruments Test Hammers
QMAKE_TARGET_COPYRIGHT = @2016 James Instruments Inc.
}
