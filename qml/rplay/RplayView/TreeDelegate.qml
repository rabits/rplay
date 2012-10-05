import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "components"

Rectangle {
    id: treeDelegate

    // private
    property bool active: current_file_array[model.level] === model.title
    onActiveChanged: {
        // Move listview to next item
        if( active && parent.parent.count !== index + 1 )
            parent.parent.currentIndex = index + 1
    }


    height: Math.max(imageSizeMin, info.height) + 8
    color: program_style.titleRectColor
    anchors {
        left: parent.left
        right: parent.right
    }

    Image {
        id: picture
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
        }

        width: imageSizeMin
        height: imageSizeMin
        source: model.picture ? model.picture : (
                    rplay_view.dataImage != "" ? rplay_view.dataImage : (
                        model.type === 'folder' ? "images/folder.png" : (
                            model.type === 'file' ? "images/file.png" : "images/album.png") ) )
        fillMode: Image.PreserveAspectFit
    }

    Item {
        id: info
        height: title.height + type.height
        anchors {
            verticalCenter: parent.verticalCenter
            left: picture.right
            right: parent.right
            leftMargin: 5
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
                pointSize: 16 * text_size
            }
            text: model.title
            color: Qt.lighter(program_style.titleColor, text_bright);
            elide: Text.ElideRight
        }

        Label {
            id: type
            anchors {
                top: title.bottom
                left: parent.left
                leftMargin: 10
            }
            font {
                pixelSize: 0
                pointSize: 12 * text_size
            }
            text: model.type
            color: Qt.lighter(program_style.typeColor, text_bright);
        }

        PlayPositionBox {
            id: playPositionBox
            visible: (model.type === 'file') && (parent.parent.active)
            height: type.height
            anchors {
                top: title.bottom
                left: type.right
                right: parent.right
                rightMargin: 10
                leftMargin: 10
            }
        }

        Label {
            id: inside
            visible: model.inside > 0
            anchors {
                top: title.bottom
                right: parent.right
                rightMargin: 10
            }
            font {
                pixelSize: 0
                pointSize: 12 * text_size
            }
            text: model.inside
            color: Qt.lighter(program_style.insideColor, text_bright);
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

    Rectangle {
        id: headerGlow
        visible: parent.active
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 0.05; color: rplay_view.colorGlow }
            GradientStop { position: 0.3; color: "transparent" }
            GradientStop { position: 0.7; color: "transparent" }
            GradientStop { position: 0.95; color: rplay_view.colorGlow }
            GradientStop { position: 1.0; color: "transparent" }
        }
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent

        onClicked: {
            if( model.type !== 'file' ) {
                mainPage.setFolder(model.path, rplay_view.dataPath);
            } else {
                ctree.shuffleClearList();
                cplayer.playFile(model.path);
            }
        }
    }

    Rectangle {
        radius: 5.0
        anchors.fill: parent
        color: program_style.foregroundColor
        opacity: 0.1
        visible: mouseArea.pressed
    }
}
