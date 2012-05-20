import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    id: directory_view

    // public
    property string dataTitle: "Music Library"
    property string dataPath: ""
    property string dataType: "folder"
    property string dataImage: ""

    property variant fill: parent

    property color  colorBackground: "transparent"
    property color  colorGlow: "#55ffffff"
    property int    imageSizeMin: 50
    property int    imageSizeMax: 150

    function start() {
        showAnimation.start()
    }

    ParallelAnimation {
        id: showAnimation
        PropertyAnimation { target: directory_view; property: "opacity"; from: 0.0; to: 1.0; duration: 200; easing.type: Easing.InOutQuad }
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

            onClicked: {
                if( dataPath !== "" && mainPage.state !== 'changePage' )
                {
                    mainPage.setFolder(ctree.parentDir(dataPath))
                }
            }

            property int pressPosX: -1

            /*onClicked: {
                console.log("click")
                if( prefsPage.state == "show" )
                {
                    redrawTree()
                }
                prefsPage.state = (prefsPage.state == "show") ? "hide" : "show"
            }*/

            onPressed: {
                pressPosX = mouseX
            }

            onMousePositionChanged: {
                mainPage.state = 'changePage'
                mainPage.pos.x += mouseX - pressPosX
            }

            onReleased: {
                if( mainPage.state === "prefsPage" )
                    prefsPage.saveSettings()

                if( mainPage.pos.x > (mainPage.width / 2) ) {
                    mainPage.state = 'prefsPage'
                }
                else if( mainPage.pos.x < -(mainPage.width / 2) ) {
                    mainPage.state = 'songPage'
                }
                else if( mainPage.state !== "prefsPage" )
                    mainPage.state = 'mainPage'

                mainPage.pos.x = -1
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
            model: ctree.treeContent(dataPath)
            ScrollDecorator {
                flickableItem: parent
            }

            delegate: Rectangle {
                id: subHeader

                // private
                property bool __active: current_file_array[model.level] === model.title

                height: Math.max(imageSizeMin, subInfo.height) + 8
                color: "transparent"
                anchors {
                    left: parent.left
                    right: parent.right
                }

                Image {
                    id: subPicture
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                    }

                    width: imageSizeMin
                    height: imageSizeMin
                    source: model.picture ? model.picture : (
                                dataImage != "" ? dataImage : (
                                    model.type === 'folder' ? "images/folder.png" : (
                                        model.type === 'file' ? "images/file.png" : "images/album.png") ) )
                    fillMode: Image.PreserveAspectFit
                }

                Item {
                    id: subInfo
                    height: subTitle.height + subType.height
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: subPicture.right
                        right: parent.right
                        leftMargin: 5
                    }

                    Label {
                        id: subTitle
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
                        id: subType
                        anchors {
                            top: subTitle.bottom
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
                        visible: (model.type === 'file') && (subHeader.__active)
                        height: subType.height
                        color: "#222"
                        radius: 2.0
                        anchors {
                            top: subTitle.bottom
                            left: subType.right
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
                            running: (model.type === 'file') && (subHeader.__active)
                            repeat: true

                            onTriggered: {
                                playPosition.anchors.rightMargin = parent.width - (parent.width * (cplayer.currentFilePosition() / cplayer.currentFileDuration()));
                                currentTime.text = parent.msec2time(cplayer.currentFilePosition());
                                durationTime.text = parent.msec2time(cplayer.currentFileDuration());
                            }
                        }
                    }

                    Label {
                        id: subInside
                        visible: model.inside > 0
                        anchors {
                            top: subTitle.bottom
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
                    id: subBorder
                    height: 1
                    color: "#444"
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                }

                Rectangle {
                    id: subHeaderGlow
                    visible: subHeader.__active
                    anchors.fill: subHeader
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "transparent" }
                        GradientStop { position: 0.05; color: directory_view.colorGlow }
                        GradientStop { position: 0.3; color: "transparent" }
                        GradientStop { position: 0.7; color: "transparent" }
                        GradientStop { position: 0.95; color: directory_view.colorGlow }
                        GradientStop { position: 1.0; color: "transparent" }
                    }
                }

                MouseArea {
                    id: subMouseArea;
                    anchors.fill: subHeader

                    onClicked: {
                        if( model.type !== 'file' ) {
                            mainPage.setFolder(model.path, dataPath)
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
                    visible: subMouseArea.pressed
                }
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
