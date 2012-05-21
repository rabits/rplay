import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    id: rplay_view

    // public
    property string dataTitle: "Rplay View"
    property string dataPath: ""
    property string dataType: ""
    property string dataImage: ""

    property variant fill: parent

    property variant   view_model
    property Component view_delegate

    property color  colorBackground: "transparent"
    property color  colorGlow: "#55ffffff"
    property int    imageSizeMin: 50
    property int    imageSizeMax: 150

    function start() {
        showAnimation.start()
    }

    ParallelAnimation {
        id: showAnimation
        PropertyAnimation { target: rplay_view; property: "opacity"; from: 0.0; to: 1.0; duration: 200; easing.type: Easing.InOutQuad }
    }

    gradient: Gradient {
        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 0.98; color: "#050505" }
        GradientStop { position: 1.0; color: "black" }
    }

    opacity: 0.0
    anchors.fill: fill

    Rectangle {
        id: header
        height: Math.max(imageSizeMax, info.height) + 8
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

            width: imageSizeMax
            height: imageSizeMax
            source: dataImage != "" ? dataImage : (dataType === 'folder' ? "images/folder.png" : "images/album.png" )
            fillMode: Image.PreserveAspectFit
        }

        Item {
            id: info
            height: title.height + type.height + path.height
            anchors {
                top: parent.top
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
                text: dataTitle
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
                text: dataType
                font {
                    pixelSize: 0
                    pointSize: 12
                }
                color: "#444"
            }

            Label {
                id: inside
                visible: list.count > 0
                anchors {
                    top: title.bottom
                    right: parent.right
                    rightMargin: 10
                }
                font {
                    pixelSize: 0
                    pointSize: 12
                }
                text: list.count
                color: "#444"
            }

            Label {
                id: path
                anchors {
                    top: type.bottom
                    left: parent.left
                    right: parent.right
                    leftMargin: 10
                }
                font {
                    pixelSize: 0
                    pointSize: 10
                }
                text: dataPath
                color: "#777"
                maximumLineCount: 4
                wrapMode: Text.Wrap
                elide: Text.ElideRight
            }
        }

        MouseArea {
            id: mouseArea;
            anchors.fill: header

            property int pressPosX: -1
            property int max_delta: 0

            onClicked: {
                if( (dataPath != "") && (Math.abs(max_delta) < 20) ) {
                    mainPage.setFolder(ctree.parentDir(dataPath));
                }
                max_delta = 0
            }

            onPressed: {
                pressPosX = mouseX
            }

            onMousePositionChanged: {
                mainPage.state = 'changePage'

                if( Math.abs(max_delta) < Math.abs(mouseX - pressPosX) )
                    max_delta = mouseX - pressPosX;
                mainPage.pos.x += mouseX - pressPosX;
            }

            onReleased: {
                if( mainPage.state === "prefsPage" )
                    prefsPage.saveSettings()
                if( Math.abs(max_delta) > 40 )
                    mainPage.switch_page(max_delta)
                else
                    mainPage.switch_page(0)
            }
        }
    }

    Rectangle {
        id: additional
        color: "transparent"
        clip: true
        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            topMargin: 4
        }

        ListView {
            id: list
            anchors.fill: parent
            model: view_model
            delegate: view_delegate
            ScrollDecorator {
                flickableItem: parent
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
}
