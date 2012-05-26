import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    id: keyvalueDelegate

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
                pointSize: 14
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
                pointSize: 18
                family: "Monospace"
            }
            text: model.value
            color: "#aaa"
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

    property variant dialog_component
    property variant dialog_sprite

    function setValueFolder() {
        value.text = cplayer.setting(model.key, dialog_sprite.selectedFile.replace("file://", ""));
        if( model.value !== value.text )
            mainPage.setFolder("/");
    }

    function changeFolder(path) {
        if( dialog_sprite != null ) {
            dialog_sprite.destroy();
            dialog_component.destroy();
        }

        dialog_component = Qt.createComponent("dialog/FsDialog.qml");
        dialog_sprite = dialog_component.createObject(prefsPage, { titleText: "Select folder"
                                                         , filterList: [""]
                                                         , currentFolder: path});
        dialog_sprite.accepted.connect(setValueFolder);
        dialog_sprite.open();
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent

        onClicked: {
            switch( model.type ) {
            case 'folder_path':
                changeFolder("file://" + value.text)
                break;
            case 'string':
                break;
            case 'text':
                break;
            case 'int':
                break;
            case 'bool':
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
