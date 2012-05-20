import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    id: fsView

    // private
    property bool __active: current_file_array[model.level] === model.title

    height: Math.max(imageSizeMin, info.height) + 8
    color: "transparent"
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
                pointSize: 16
            }
            text: model.title
            color: "gray"
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
                pointSize: 12
            }
            text: model.type
            color: "#444"
        }

        Rectangle {
            id: playPositionBox
            visible: (model.type === 'file') && (parent.parent.__active)
            height: type.height
            color: "#222"
            radius: 2.0
            anchors {
                top: title.bottom
                left: type.right
                right: parent.right
                rightMargin: 10
                leftMargin: 10
            }

            function two(x) {return ((x>9)?"":"0")+x}
            function msec2time(msec) {
                msec = Math.floor(msec / 1000)
                var time = "00:00";

                if( msec > 0 )
                {
                    time = two(msec % 60);

                    msec = Math.floor(msec / 60)
                    time = two(msec % 60) + ":" + time;

                    msec = Math.floor(msec / 60)
                    if( msec > 0 )
                        time = (msec % 24) + ":" + time;
                }

                return time;
            }

            Label {
                id: currentTime
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 5
                }
                font.pixelSize: parent.height - 4
                color: "#999"
            }

            Label {
                id: durationTime
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 5
                }
                font.pixelSize: parent.height - 4
                color: "#999"
            }

            Rectangle {
                id: playPosition
                color: "transparent"
                anchors {
                    fill: parent
                    rightMargin: parent.width
                }

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.1; color: "transparent" }
                    GradientStop { position: 0.42; color: colorGlow }
                    GradientStop { position: 0.58; color: colorGlow }
                    GradientStop { position: 0.9; color: "transparent" }
                    GradientStop { position: 1.0; color: "transparent" }
                }
            }

            Timer {
                interval: 200
                running: (model.type === 'file') && (parent.parent.parent.__active)
                repeat: true

                onTriggered: {
                    playPosition.anchors.rightMargin = parent.width - (parent.width * (cplayer.currentFilePosition() / cplayer.currentFileDuration()));
                    currentTime.text = parent.msec2time(cplayer.currentFilePosition());
                    durationTime.text = parent.msec2time(cplayer.currentFileDuration());
                }
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
                pointSize: 12
            }
            text: model.inside
            color: "#444"
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

    Rectangle {
        id: headerGlow
        visible: parent.__active
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
                mainPage.setFolder(model.path, rplay_view.dataPath)
            } else {
                cplayer.playFile(model.path)
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
