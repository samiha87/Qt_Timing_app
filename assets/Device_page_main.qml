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
import "widgets"

Rectangle {
    id: devicePage
    width: 300
    height: 600
    property variant window;
    property bool popupSettingsShown: false;
    property bool confirmPopupShown: false;

    Component.onCompleted: {
        if (device.controllerError) {
            info.visible = false;
            menu.menuText = device.update
        }
    }


    Header {
        id: header
        anchors.top: parent.top
        headerText: "SpeedUp"
    }

    Device_status_bar {
        id: device_status
        anchors.top: header.bottom
        batteryLevel: timeManagement.battery
        deviceType: timeManagement.type
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
        anchors.top: device_status.bottom
        anchors.bottom: menu.top
        model: timeManagement.timeSlotListQml
        clip: true

        delegate: Rectangle {
            id: timeSlot
            height:152
            //color: "lightsteelblue"
            color: modelData.getTimeSlotState
            border.width: 2
            border.color: "black"
            radius: 5
            width: parent.width
            Component.onCompleted: {
                info.visible = false
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {

                }
            }

            Text {
                id: timeSlotTime
                font.pointSize: 40
                font.bold: modelData.isRunning
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#363636"
                horizontalAlignment: Text.AlignHCenter
                text: modelData.getTimeFromSlot
            }

            Text {
                id: participantIdentity
                font.pointSize: 10
                font.bold: modelData.isRunning
                anchors.left: timeSlotTime.right
                anchors.leftMargin: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                color: "#363636"
                horizontalAlignment: Text.AlignHCenter
                text: modelData.getParticipant
            }
        }
    }

    Command_menu {
        id: btn_settings
        z: 53
        anchors.left: parent.left
        anchors.bottom: menu.top
        menuWidth: parent.width/3
        menuHeight: parent.height/8
        menuText: "Menu"
        visible: true
        onCommandButtonClick: {
           if(popupSettings.visible === true) {
               popupSettings.visible = false
           } else {
                popupSettings.visible = true;
           }
        }
    }

    Command_menu {
        id: btn_skip
        z: 54
        anchors.right: btn_reset.left
        anchors.bottom: menu.top
        menuWidth: parent.width/3
        menuHeight: (parent.height/8)
        menuText: "Skip time"

        onCommandButtonClick: {
            device.transmitData("Skip");
        }
    }

    Command_menu {
        id: btn_reset
        z: 55
        //anchors.left: btn_skip.right
        anchors.bottom: menu.top
        anchors.right: parent.right
        menuWidth: parent.width/3
        menuHeight: parent.height/8
        menuText: "Reset"

        onCommandButtonClick: {
            if(!confirmPopupShown) {
                confirmPopupShown = true;
                confirmPopup.visible = true;
            } else {
                confirmPopupShown = false;
                confirmPopup.visible = false;
            }
            //device.transmitData("h!1");
        }
    }

    Menu {
        id: menu
        z: 52
        anchors.bottom: parent.bottom
        menuWidth: parent.width
        menuText: device.update
        menuHeight: (parent.height / 8)
        onButtonClick: {
            device.update = "Search"
            device.disconnectFromDevice();
            pageLoader.source = "" // Closes current page

        }
    }

    PopupSettings {
        id: popupSettings
        width: btn_settings.width
        height: (btn_settings.height * 4)
        anchors.left: btn_settings.left
        anchors.right: btn_settings.right
        anchors.bottom: btn_settings.top
        visible: false
    }

    Confirmation_popup {
        id: confirmPopup
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        visible: false
        onClosePopup: {
            confirmPopupShown = false;
            confirmPopup.visible = false;
        }
    }
}
