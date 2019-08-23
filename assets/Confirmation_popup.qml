import QtQuick 2.0

Rectangle {
    id: container
    width: parent.width
    height: parent.height/5
    signal closePopup();
    Rectangle {
        z: 0
        width: parent.width
        height:parent.height
        color: "white"
    }

    Text{
        id: confirmText
        text: "Do you want to reset times?"
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 25
    }

  Command_menu {
      id: btn_confirm
      anchors.left: container.left
      anchors.leftMargin: 20
      anchors.verticalCenter: parent.verticalCenter


      menuWidth: container.width/2.3
      menuHeight: container.height/3
      menuText: "Yes"
      onCommandButtonClick: {
          device.transmitData("Reset");
          closePopup();
      }
  }

  Command_menu {
      id: btn_cancel
      anchors.right: container.right
      anchors.rightMargin: 20
      anchors.verticalCenter: parent.verticalCenter

      menuWidth: container.width/2.3
      menuHeight: container.height/3
      menuText: "No"
      onCommandButtonClick: {
          closePopup(); // Close popup
      }
  }
}
