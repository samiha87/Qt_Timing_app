/* History page, this page shows current history
*
*/

import QtQuick 2.0
import "widgets"

Rectangle {
    id: history_page
    width: 300
    height: 600

    property variant window;

    Header {
        id: header
        anchors.top: parent.top
        headerText: "Totem: History"
    }

    ListView {
        id: history_list
        width: parent.width
        anchors.top: header.bottom
        model: historyManager.historyList
        clip: true

        delegate: Rectangle {
            id: time_slot
            height: 152
            color: modelData.getTimeSlotState
            border.width: 2
            border.color: "black"
            radius: 5
            width: parent.width

            MouseArea {
                anchors.fill: parent
                onClicked: {

                }
            }

            // Shows lap for the run
            Label {
                id: rider_identity
                textContent: modelData.getIdentityFromSlot
                font.bold: modelData.isRunning
                font.pointSize: (time_slot.height * 0.2)
                anchors.top: time_slot.top
                anchors.topMargin: 5
                anchors.left: time_slot.left
                anchors.leftMargin: 5
            }

            Label {
                id: lap
                textContent: modelData.getLapFromSlot
                font.bold: modelData.isRunning
                font.pointSize: (time_slot.height * 0.2)
                anchors.top: time_slot.top
                anchors.topMargin: 5
            }

            // Shows time of the run
            Label {
                id: time
                textContent: modelData.getTimeFromSlot
                font.bold: modelData.isRunning
                font.pointSize: (time_slot.height * 0.2)

                anchors.top: time_slot.top
                anchors.topMargin: 5
                anchors.left:lap.right
                anchors.leftMargin: 5
            }
        }
    }
}
