import QtQuick 2.2

Rectangle {
    id: root
    property bool gameEnded: false
    property string winner: ""
    width: 340
    height: 400

    signal clicked

    signal tryAgain

    onWinnerChanged: outcome.text = winner

    Grid {
        id: grid
        anchors.top: root.top
        anchors.left: root.left
        anchors.right: root.right
        anchors.margins: 20
        spacing: 1

        columns: 3
        rows: 3

        Repeater {
            model: 9

            Box {
                myIndex: index
                onBoxClicked: {
                    clickable = false
                    root.clicked()
                }
            }
        }
    }

    Text {
        id: outcome
        font.pixelSize: 30
        anchors.top: grid.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

    }

    Text
    {
        id: button
        visible: gameEnded

        anchors.top: outcome.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottomMargin: 10
        font.pixelSize: 30
        font.bold: true
        color: "#0a49d1"

        text: "Try Again"

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        MouseArea {
            anchors.fill: parent
            onClicked: root.tryAgain()

        }
    }
}
