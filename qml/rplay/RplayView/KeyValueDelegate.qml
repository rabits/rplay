import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "dialog"
import "components"

Rectangle {
    id: keyvalueDelegate

    property bool editModeOn: false

    height: title.height + Math.max(viewValue.height, (sprite != null ? sprite.height : 0)) + 8
    color: "transparent"
    anchors {
        left: parent.left
        right: parent.right
        leftMargin: 5
        rightMargin: 5
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent

        onClicked: {
            switch( model.type ) {
            case 'folder_path':
                changeFolder("file://" + viewValue.text)
                break;
            case 'lyric':
                changeText();
                break;
            case 'bool':
                if (viewValue.text == "true")
                    setValueBool(false)
                else
                    setValueBool(true);
                break;
            case 'about':
                showInfo(qsTr("About and Help"), cplayer.helpContent());
                break;
            }
        }
    }

    Rectangle {
        id: titleRect
        height: title.height
        color: "#22ffffff"
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            leftMargin: 5
        }
        Label {
            id: title
            font {
                weight: Font.Bold
                pixelSize: 0
                pointSize: 14
            }
            text: model.title
            color: "#888"
            elide: Text.ElideRight
            onLinkActivated: Qt.openUrlExternally(link);
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

    property variant sprite

    Component {
        id: folderDialog
        FsDialog {}
    }

    Component {
        id: editTextArea
        EditTextArea {
            width: keyvalueDelegate.width
            onFocused: keyvalueDelegate.parent.parent.parent.parent.headerMini();
            onUnfocused: keyvalueDelegate.parent.parent.parent.parent.headerMaxi();
        }
    }

    function setValueFolder() {
        var text = sprite.selectedFile.replace("file://", "");
        if( model.value !== text )
        {
            ListView.view.model.setData(index, "value", cplayer.setting(model.key, text));
            mainPage.setFolder("/");
        }
    }

    function changeFolder(path) {
        if( sprite != null ) {
            sprite.destroy(100);
        }

        sprite = folderDialog.createObject(keyvalueDelegate.parent.parent.parent.parent
                                          , { titleText: "Select folder"
                                            , filterList: [""]
                                            , currentFolder: path });
        sprite.accepted.connect(setValueFolder);
        sprite.open();
    }

    function saveText(text) {
        ListView.view.model.setData(index, "title", "Lyrics")
        ListView.view.model.setData(index, "value", text)
        clyrics.lyricSave(text);
    }

    function changeText() {
        if( sprite != null ) {
            sprite.destroy(100);
        }

        sprite = editTextArea.createObject(keyvalueDelegate
                                          , { text: model.value
                                            , y: viewValue.y });
        editModeOn = true;
    }

    function setValueBool(bool) {
        ListView.view.model.setData(index, "value", cplayer.setting(model.key, bool));
    }

    Button {
        id: saveButton
        visible: editModeOn
        anchors.top: titleRect.top
        anchors.right: titleRect.right
        height: titleRect.height
        width: 100
        text: "save"
        onClicked: {
            editModeOn = false;
            saveText(sprite.text);
            sprite.height = 0;
            sprite.destroy(100);
        }
        style: ButtonStyle { inverted: true }
    }

    Button {
        id: cancelButton
        visible: editModeOn
        anchors.top: titleRect.top
        anchors.right: saveButton.left
        height: titleRect.height
        width: 100
        text: "cancel"
        onClicked: {
            editModeOn = false;
            sprite.height = 0;
            sprite.destroy(100);
        }
        style: ButtonStyle { inverted: true }
    }

    Text {
        id: viewValue
        visible: !editModeOn
        anchors {
            top: titleRect.bottom
            left: parent.left
            right: parent.right
            leftMargin: 15
        }
        font {
            pixelSize: 0
            pointSize: 18
            family: "Monospace"
        }
        text: model.value
        wrapMode: Text.Wrap
        color: "#999"
        onLinkActivated: Qt.openUrlExternally(link);
    }

    Rectangle {
        radius: 5.0
        anchors.fill: parent
        color: "white"
        opacity: 0.1
        visible: mouseArea.pressed
    }
}
