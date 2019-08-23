import QtQuick 2.0

Rectangle {
    id: container
    width: 300
    height: 300

    property bool startActive: false
    property bool splitActive: false
    property bool endActive: false
    property bool masterActive: false

    Command_menu {
        id: btn_wifi
        anchors.top: container.top
        anchors.left: container.left
        menuWidth: container.width
        menuHeight: container.height/2
        //darken: timeManagement.getStartActive
        menuText: "Wi-Fi"
        onCommandButtonClick: {
             pageLoader.source = "WiFi_page.qml"
        }
        visible: true
    }

    Command_menu {
        id: btn_configure
        anchors.top: btn_wifi.bottom
        anchors.left: container.left
        menuWidth: container.width
        menuHeight: container.height/2
        //darken: timeManagement.getStartActive
        menuText: "Configurations"
        onCommandButtonClick: {
            //device.setDeviceType("Start");
        }
        visible: true
    }
}

