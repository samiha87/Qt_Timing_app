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
        id: btn_setStart
        anchors.top: container.top
        anchors.left: container.left
        menuWidth: container.width
        menuHeight: container.height/2
        darken: timeManagement.getStartActive
        menuText: "Start"
        onCommandButtonClick: {
            device.setDeviceType("Start");
        }
    }

    Command_menu {
        id: btn_setSplit

        anchors.top: btn_setStart.bottom
        anchors.left: container.left
        menuWidth: container.width
        menuHeight: container.height/4
        darken: timeManagement.getSplitActive
        menuText: "Split"
        onCommandButtonClick: {
            device.setDeviceType("Split");
        }
        visible: false
    }

    Command_menu {
        id: btn_setEnd
        anchors.top: btn_setStart.bottom
        anchors.left: container.left

        menuWidth: container.width
        menuHeight: container.height/2
        menuText: "End"
        darken: timeManagement.getEndActive
        onCommandButtonClick: {
            device.setDeviceType("End");
        }
    }

    Command_menu {
        id: btn_setMaster

        anchors.left: container.left
        anchors.bottom: container.bottom

        menuWidth: container.width
        menuHeight: container.height/4
        menuText: "Master"
        darken: timeManagement.getMasterActive
        onCommandButtonClick: {
            device.setDeviceType("Master");
        }
        visible: false
    }
}

