import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "dialog"
import "components"

Rectangle {
    id: keyvalueDelegate

    property bool editModeOn: false

    height: title.height + (editModeOn ? sprite.height : viewValue.height) + 8
    color: "transparent"
    anchors {
        left: parent.left
        right: parent.right
        leftMargin: 5
        rightMargin: 5
    }

    Component.onCompleted: {
        if( model.type === "slider" )
        {
            sprite = compSlider.createObject(keyvalueDelegate
                                            , { value: model.value });
            editModeOn = true;
            saveButton.visible = false;
            cancelButton.visible = false;
        }
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
        color: program_style.titleRectColor
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
                pointSize: 14 * text_size
            }
            text: model.title
            color: Qt.lighter(program_style.titleColor, text_bright);
            elide: Text.ElideRight
            onLinkActivated: Qt.openUrlExternally(link);
        }
    }

    Rectangle {
        id: border
        height: 1
        color: program_style.borderColor
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }

    property variant sprite
    property bool useEditValueHeight: false

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

    Component {
        id: compSlider
        Slider {
            anchors {
                top: titleRect.bottom
                left: parent.left
                right: parent.right
                leftMargin: 15
            }
            value: model.value
            minimumValue: 0.05
            maximumValue: 2.0
            stepSize: 0.05
            valueIndicatorVisible: true
            onValueChanged: {
                setValueInt(value);
            }
        }
    }

    function setValueInt(value) {
        ListView.view.model.setData(index, "value", value);

        // TODO: UGLY HACK
        if( model.key === "preferences/text_bright" )
            text_bright = cplayer.setting(model.key, value);
        if( model.key === "preferences/text_size" )
            text_size = cplayer.setting(model.key, value);
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

    function setValueBool(value) {
        ListView.view.model.setData(index, "value", cplayer.setting(model.key, value));

        // TODO: UGLY HACK
        if( model.key === "preferences/style_inverse" )
            program_style.inverted = value;
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
            pointSize: 18 * text_size
            family: "Monospace"
        }
        text: model.value
        wrapMode: Text.Wrap
        color: Qt.lighter(program_style.viewValueColor, text_bright);
        onLinkActivated: Qt.openUrlExternally(link);
    }

    Rectangle {
        radius: 5.0
        anchors.fill: parent
        color: program_style.endListBorderColor
        opacity: 0.1
        visible: mouseArea.pressed
    }
}
