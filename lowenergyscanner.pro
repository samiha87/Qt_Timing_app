TARGET = Totem
INCLUDEPATH += .

QT += quick bluetooth
QT += network
QT += androidextras

# Input
SOURCES += main.cpp \
    device.cpp \
    deviceinfo.cpp \
    serviceinfo.cpp \
    characteristicinfo.cpp \
    timeslot.cpp \
    timemanagement.cpp \
    device_status.cpp \
    memoryhandler.cpp \
    serverthread.cpp \
    wifihandler.cpp

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
    memoryhandler.h \
    serverthread.h \
    wifihandler.h

RESOURCES += \
    resources.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/lowenergyscanner
INSTALLS += target

DISTFILES += \
    assets/Confirmation_popup.qml \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    assets/widgets/TimeSlot.qml \
    assets/WiFi_page.qml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android




