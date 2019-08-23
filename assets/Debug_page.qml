/***************************************************************************
**
** Copyright (C) 2013 BlackBerry Limited. All rights reserved.
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0

Rectangle {
    id: devicePage
    width: 300
    height: 600
    property variant window;
    property bool popupSettingsShown: false;

    Component.onCompleted: {
        if (device.controllerError) {
            info.visible = false;
            menu.menuText = device.update
        }
    }

    Header {
        id: header
        anchors.top: parent.top
        headerText: "Massive Timing portal"
        fontSize: 10;
        headerHeight: 20
        logoVisibe: false
    }

    Dialog {
        id: info
        anchors.centerIn: parent
        visible: true
        dialogText: "Connecting to device...";
    }

    Connections {
        target: device
        onServicesUpdated: {
            if (servicesview.count == 0)
                info.dialogText = "Cannot connect"
            else
                info.visible = false;
                // Start transmission
                //device.transmitData("!c") // 0x21, 0x63
        }
    }

    ListView {
        id: servicesview
        width: parent.width
        anchors.top: header.bottom
        height: 150
        model: timeManagement.timeSlotListQml
        clip: true

        delegate: Rectangle {
            id: timeSlot
            height: 150
            color: "lightsteelblue"
            border.width: 2
            border.color: "black"
            radius: 5
            width: parent.width
            Component.onCompleted: {
                info.visible = false
            }

            MouseArea {
                anchors.bottomMargin: 82
                anchors.fill: parent
                onClicked: {

                }
            }

            Label {
                id: timeSlotTime
                width: 300
                textContent: modelData.getTimeFromSlot
                font.bold: modelData.isRunning
                font.pointSize: (timeSlot.height * 0.2)
                anchors.bottom: timeSlot.bottom
                anchors.bottomMargin: 49
            }
        }
    }

    ListView {
        id: debug_window
        width:parent.width
        anchors.top:servicesview.bottom
        anchors.bottom: btn_skip.top
        model: timeManagement.timeSlotListQml
        clip: true

        delegate: Rectangle {
            id: debug_slot
            height: 150
            color: "lightsteelblue"
            border.width: 2
            border.color: "black"
            radius: 5
            width: parent.width
            Component.onCompleted: {
                info.visible = false
            }

            MouseArea {
                anchors.bottomMargin: 82
                anchors.fill: parent
                onClicked: {

                }
            }

            Label {
                id: debug_text_slot
                width: 300
                textContent: modelData.getTimeFromSlot
                font.bold: modelData.isRunning
                font.pointSize: (timeSlot.height * 0.2)
                anchors.bottom: timeSlot.bottom
                anchors.bottomMargin: 49
            }
        }
    }

    Menu {
        id: menu
        anchors.bottom: parent.bottom
        menuWidth: parent.width
        menuText: device.update
        menuHeight: (parent.height / 6)
        onButtonClick: {
            //device.transmitData("!d")
            device.disconnectFromMtGate()
            device.update = "Search"
            pageLoader.source = "" // Closes current page
        }
    }

   Command_menu {
        id: btn_settings
        anchors.left: parent.left
        anchors.bottom: menu.top
        menuWidth: parent.width/3
        menuHeight: parent.height/7
        menuText: "Settings"

        onCommandButtonClick: {
            if(!popupSettingsShown) {
                popupSettingsShown = true;
                popupSettings.visible = true;
            } else {
                popupSettingsShown = false;
                popupSettings.visible = false;
            }

            // open settings menu
        }
    }
    /*
    Command_menu {
            id: btn_history
            anchors.left: parent.left
            anchors.bottom: menu.top
            menuWidth: parent.width/3
            menuHeight: parent.height/7
            menuText: "History"

            onCommandButtonClick: {
               pageLoader.source = "History.qml"
            }
        }
    */
    Command_menu {
        id: btn_skip
        anchors.left: btn_settings.right
        anchors.bottom: menu.top
        menuWidth: parent.width/3
        menuHeight: (parent.height/7)
        menuText: "Skip time"

        onCommandButtonClick: {
            device.transmitData("Skip");
            // Enable upload
        }
    }

    Command_menu {
        id: btn_alarm
        anchors.left: btn_skip.right
        anchors.bottom: menu.top
        menuWidth: parent.width/3
        menuHeight: parent.height/7
        menuText: "Reset"

        onCommandButtonClick: {
            // open confirmation window
            device.transmitData("Reset");
        }
    }

    PopupSettings {
        id: popupSettings
        width: btn_settings.width
        height: (btn_settings.height * 3)
        anchors.left: btn_settings.left
        anchors.right: btn_settings.right
        anchors.bottom: btn_settings.top
        visible: false
    }
}
