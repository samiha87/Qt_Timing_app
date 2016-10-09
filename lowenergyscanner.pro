TARGET = MT-Portal
INCLUDEPATH += .

QT += quick bluetooth

# Input
SOURCES += main.cpp \
    device.cpp \
    deviceinfo.cpp \
    serviceinfo.cpp \
    characteristicinfo.cpp \
    timeslot.cpp \
    timemanagement.cpp \
    device_status.cpp \
    memoryhandler.cpp

OTHER_FILES += assets/*.qml \
    assets/widgets/*.qml \
    assets/widgets/Device_status_bar.qml \
    images/MassiveTiming.png \
    images/battery/*.png

HEADERS += \
    device.h \
    deviceinfo.h \
    serviceinfo.h \
    characteristicinfo.h \
    mt_device_defines.h \
    timeslot.h \
    timemanagement.h \
    device_status.h \
    memoryhandler.h

RESOURCES += \
    resources.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/lowenergyscanner
INSTALLS += target



