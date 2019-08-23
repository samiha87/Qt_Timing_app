import QtQuick 2.0

Rectangle {
    property string textContent1: ""
    property alias labelWidth: labelText1.width
    property string textContent2: ""
    property alias labelWidth: labelText2.width
    property alias pointSize1: labelText1.font.pointSize
    property alias pointSize2: labelText2.font.pointSize

    height: parent.height
    color: "lightsteelblue"
    //color: parent.color
    radius: 5
    width: parent.width

    Text {
        id: labelText1
        font.pointSize: 20
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#363636"
        horizontalAlignment: Text.AlignHCenter
        anchors.verticalCenter: parent.verticalCenter
        elide: Text.ElideMiddle
        width: parent.width/3
        wrapMode: Text.Wrap
        text: textContent
    }

    Text {
        id: labelText2
        font.pointSize: 20
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#363636"
        horizontalAlignment: Text.AlignHCenter
        anchors.right: parent.right
        anchors.rightMargin: 5;
        elide: Text.ElideMiddle
        width: parent.width/3
        wrapMode: Text.Wrap
        text: textContent
    }
}
