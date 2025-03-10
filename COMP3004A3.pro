QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addbehaviourdialog.cpp \
    addeventdialog.cpp \
    addpassengerdialog.cpp \
    behaviour.cpp \
    door.cpp \
    elevator.cpp \
    main.cpp \
    mainwindow.cpp \
    passenger.cpp \
    safetyevent.cpp \
    simulationcontroller.cpp

HEADERS += \
    addbehaviourdialog.h \
    addeventdialog.h \
    addpassengerdialog.h \
    behaviour.h \
    door.h \
    elevator.h \
    mainwindow.h \
    passenger.h \
    safetyevent.h \
    simulationcontroller.h

FORMS += \
    addbehaviourdialog.ui \
    addeventdialog.ui \
    addpassengerdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
