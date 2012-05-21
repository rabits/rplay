import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "dialog"

Rectangle {
    id: prefDelegate

    height: info.height + 8
    color: "transparent"
    anchors {
        left: parent.left
        right: parent.right
        leftMargin: 5
        rightMargin: 5
    }

    Item {
        id: info
        height: title.height + value.height
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }

        Label {
            id: title
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                leftMargin: 5
            }
            font {
                weight: Font.Bold
                pixelSize: 0
                pointSize: 16
            }
            text: model.title
            color: "#888"
            elide: Text.ElideRight
        }

        Label {
            id: value
            anchors {
                top: title.bottom
                left: parent.left
                leftMargin: 15
            }
            font {
                pixelSize: 0
                pointSize: 12
            }
            text: model.value
            color: "#555"
            wrapMode: Text.Wrap
        }
    }

    Rectangle {
        id: border
        height: 1
        color: "#444"
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }

    FsDialog {
        id: selectFolderDialog
        titleText: "Select folder"
        filterList: [""]
        currentFolder: "file://" + value.text
        onAccepted: {
            value.text = cplayer.setting(model.key, selectedFile.replace("file://", ""))
            selectFolderDialog.currentFolder = value.text
        }
    }

    Dialog {
        id: textEditDialog
    }

    Dialog {
        id: numberEditDialog
    }

    Dialog {
        id: boolEditDialog
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent

        onClicked: {
            switch( model.type ) {
            case 'path':
                selectFolderDialog.open()
                break;
            case 'text':
                textEditDialog.show()
                break;
            case 'int':
                numberEditDialog.show()
                break;
            case 'bool':
                boolEditDialog.show()
                break;
            }
        }
    }

    Rectangle {
        radius: 5.0
        anchors.fill: parent
        color: "white"
        opacity: 0.1
        visible: mouseArea.pressed
    }
}
