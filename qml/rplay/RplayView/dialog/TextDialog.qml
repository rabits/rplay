import QtQuick 1.1

Rectangle {
    id: infoDialog

    property string titleText: "default title"
    property string contentText: "default content"

    anchors.fill: parent
    visible: false
    opacity: 0.0
    color: program_style.infoDialogColor

    function show() {
        state = "visible";
    }

    function hide() {
        state = "";
    }

    Rectangle {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: titleData.height
        color: program_style.titleRectColor
        radius: 10
        Text {
            id: titleData
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 0
            font.pointSize: 20 * text_size
            color: Qt.lighter(program_style.titleDataColor, text_bright);
            text: titleText
        }
    }

    MouseArea {
        id: area
        anchors.fill: infoDialog;
        onClicked: hide();
    }

    Rectangle {
        id: content
        anchors.top: title.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20
        color: program_style.titleRectColor
        radius: 10
        Flickable {
            flickableDirection: Flickable.VerticalFlick
            contentHeight: contentData.height
            anchors.fill: parent
            clip: true
            Text {
                id: contentData
                anchors.left: parent.left
                anchors.right: parent.right
                wrapMode: Text.WordWrap
                font.pixelSize: 0
                font.pointSize: 16 * text_size
                color: Qt.lighter(program_style.contentDataColor, text_bright);
                text: contentText
                onLinkActivated: Qt.openUrlExternally(link);
            }
        }
    }

    states: [
        State {
            name: "visible"
            PropertyChanges{ target: infoDialog; opacity: 1.0 }
            PropertyChanges{ target: infoDialog; visible: true }
        }
    ]

    transitions: [
        Transition {
            reversible: true;
            NumberAnimation { properties: "opacity"; duration: 400 }
            NumberAnimation { properties: "visible"; duration: 400 }
        }
    ]
}
