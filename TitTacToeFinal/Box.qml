import QtQuick 2.0

Rectangle {

    property int myIndex
    property int owner: 0
    property bool clickable: true

    signal boxClicked

    width: 100
    height: 100
    border.width: 2
    radius: 10

    border.color: "#606060"

    onOwnerChanged: {

        switch (owner) {
        case 0:
            oBox.visible = false
            xBox.visible = false
            break
        case 1:
            oBox.visible = false
            xBox.visible = true
            break
        case -1:
            xBox.visible = false
            oBox.visible = true
            break
        default:
            console.log("error owner")
        }
    }

    Image {
        id: oBox
        anchors.fill: parent
        anchors.margins: 5
        source: "O.jpg"
        visible: false
    }

    Image {
        id: xBox
        anchors.fill: parent
        anchors.margins: 5
        source: "X.jpg"
        visible: false
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (clickable) {
                owner = -1
                boxClicked()
            }
        }
    }
}
