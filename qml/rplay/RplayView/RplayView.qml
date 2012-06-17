import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "components"

Rectangle {
    id: rplay_view

    // public
    property string dataTitle: "Rplay View"
    property string dataPath: ""
    property string dataType: ""
    property string dataImage: ""

    property string dataNoItemsMessage: "No items do display"

    property variant   view_model
    property Component view_delegate

    property bool longPressUse: false
    property bool positionBoxUse: false
    property bool playPauseOnCoverUse: false

    signal clicked

    property color  colorBackground: "transparent"
    property color  colorGlow: "#55ffffff"
    property int    imageSizeMin: 50
    property int    imageSizeMax: 150
    property int    titleSize: 16

    function start() {
        showAnimation.start()
    }

    function headerMini() {
        header.state = "mini"
    }

    function headerMaxi() {
        header.state = ""
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
    anchors.fill: parent

    Image {
        id: backgroundImage
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        opacity: 0.1
        width: 300
        height: 300

        source: "images/rabit.png"

        fillMode: Image.PreserveAspectFit
    }

    Rectangle {
        id: header
        height: Math.max(picture.height, info.height) + 8
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

            SequentialAnimation {
                id: picSplash
                NumberAnimation { target: picture; properties: "opacity"; from: 1.0; to: 0.5; duration: 100 }
                NumberAnimation { target: picture; properties: "opacity"; from: 0.5; to: 1.0; duration: 100 }
            }

            MouseArea {
                id: pictureMouseArea;
                anchors.fill: parent

                onClicked: {
                    cplayer.playFile(cplayer.currentFile());
                    picSplash.start();
                }
            }
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
                    pointSize: titleSize
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
                    pointSize: 16
                }
                color: "#555"
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
                    pointSize: 16
                }
                text: list.count
                color: "#555"
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
                    pointSize: 12
                }
                text: dataPath
                color: "#777"
                maximumLineCount: 4
                wrapMode: Text.WrapAnywhere
                elide: Text.ElideRight
            }
        }

        Image {
            id: repeatFolderImage
            fillMode: Image.PreserveAspectFit
            visible: dataPath === cplayer.setting("preferences/repeat_shuffle_folder")

            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 10

            source: "image://theme/icon-m-toolbar-repeat-white"
        }

        Image {
            id: statusVoice
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 10
            fillMode: Image.PreserveAspectFit
            visible: false

            Connections {
                target: cvoice

                onWorkingChanged: {
                    statusVoice.visible = cvoice.working;
                }
            }

            source: "images/refresh_voice.png"
            SequentialAnimation on opacity {
                running: visible
                loops: Animation.Infinite
                NumberAnimation { from: 1.0; to: 0.5; duration: 500; easing.type: Easing.InOutBounce }
                NumberAnimation { from: 0.5; to: 1.0; duration: 500; easing.type: Easing.InOutBounce }
            }
        }

        Image {
            id: statusLyrics
            anchors.top: parent.top
            anchors.right: statusVoice.left
            anchors.margins: 10
            fillMode: Image.PreserveAspectFit
            visible: false

            Connections {
                target: clyrics

                onWorkingChanged: {
                    statusLyrics.visible = clyrics.working;
                }
            }

            source: "images/refresh_lyrics.png"
            SequentialAnimation on opacity {
                running: visible
                loops: Animation.Infinite
                NumberAnimation { from: 1.0; to: 0.5; duration: 500; easing.type: Easing.InOutBounce }
                NumberAnimation { from: 0.5; to: 1.0; duration: 500; easing.type: Easing.InOutBounce }
            }
        }

        Image {
            id: statusCover
            anchors.top: parent.top
            anchors.right: statusLyrics.left
            anchors.margins: 10
            fillMode: Image.PreserveAspectFit
            visible: false

            Connections {
                target: ccover

                onWorkingChanged: {
                    statusCover.visible = ccover.working;
                }
            }

            source: "images/refresh_cover.png"
            SequentialAnimation on opacity {
                running: visible
                loops: Animation.Infinite
                NumberAnimation { from: 1.0; to: 0.5; duration: 500; easing.type: Easing.InOutBounce }
                NumberAnimation { from: 0.5; to: 1.0; duration: 500; easing.type: Easing.InOutBounce }
            }
        }

        MouseArea {
            id: mouseArea;
            anchors.left: playPauseOnCoverUse ? picture.right : header.left
            anchors.right: header.right
            anchors.top: header.top
            anchors.bottom: header.bottom

            property int pressPosX: -1
            property int pageX: -1
            property int max_delta: 0

            onClicked: {
                if( Math.abs(max_delta) < 10 ) {
                    rplay_view.clicked()
                }
                max_delta = 0
            }

            onPressed: {
                pressPosX = mouseX
                pageX = mainPage.pos.x;
                headerMaxi();
            }

            onPressAndHold: {
                if( longPressUse && Math.abs(max_delta) < 20 )
                {
                    cplayer.setting("preferences/repeat_shuffle_folder", dataPath);
                    repeatFolderImage.visible = true;
                    showBanner(qsTr("Folder %1 will be used for repeat and shuffle modes").arg(dataTitle), 3000, "image://theme/icon-m-toolbar-repeat-white");
                }
            }

            onMouseXChanged: {
                mainPage.state = 'changePage'

                if( Math.abs(max_delta) < Math.abs(mouseX - pressPosX) )
                    max_delta = mouseX - pressPosX;
                if( Math.abs(max_delta) > 10 )
                    mainPage.pos.x += mouseX - pressPosX;
            }

            onReleased: {
                if( Math.abs(max_delta) > 30 )
                    mainPage.switch_page(max_delta)
                else if( Math.abs(mainPage.pos.x - pageX) > 50 )
                    mainPage.switch_page(mainPage.pos.x - pageX)
                else
                    mainPage.switch_page(0)
            }
        }

        PlayPositionBox {
            id: playPositionBox
            useChangePosition:true
            visible: positionBoxUse
            height: 40
            anchors {
                bottom: parent.bottom
                left: picture.right
                right: parent.right
                rightMargin: 10
                leftMargin: 10
            }
        }

        states: [
            State {
                name: "mini"
                PropertyChanges{ target: picture; width: title.height; height: title.height }
                PropertyChanges{ target: type; visible: false; height: 0 }
                PropertyChanges{ target: inside; visible: false; height: 0 }
                PropertyChanges{ target: path; visible: false; height: 0 }
                PropertyChanges{ target: playPositionBox; anchors.leftMargin: ((rplay_view.width - title.paintedWidth) > 200) ? title.paintedWidth + 20 : 200 }
            }
        ]

        transitions: [
            Transition {
                SequentialAnimation {
                    NumberAnimation { properties: "width, height, anchors.leftMargin"; duration: 200 }
                    NumberAnimation { properties: "visible"; duration: 0 }
                }
            }
        ]
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

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            visible: list.visible ? false : true
            text: "<center>" + dataNoItemsMessage + "</center>"
            font {
                pixelSize: 0
                pointSize: 30
            }
            wrapMode: Text.WordWrap
            maximumLineCount: 4
            color: "#55ffffff"
        }

        ListView {
            id: list
            anchors.fill: parent
            model: view_model
            delegate: view_delegate
            visible: count > 0 ? true : false
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
