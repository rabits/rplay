import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: mainPage

    states: [
        State {
            name: "mainPage"
            PropertyChanges{ target: mainPage; pos.x: 0 }
        },
        State {
            name: "treePage"
            PropertyChanges{ target: mainPage; pos.x: mainPage.width }
        },
        State {
            name: "songPage"
            PropertyChanges{ target: mainPage; pos.x: -mainPage.width }
        },
        State {
            name: "changePage"
            PropertyChanges{ target: mainPage; pos.x: mainPage.x }
        }
    ]

    transitions: [
        Transition {
            to: "changePage"
        },
        Transition {
            NumberAnimation { properties: "pos.x"; duration: 100 }
        }
    ]

    MouseArea {
        id: pagesChanger
        height: parent.height
        width: parent.width * 3
        anchors.centerIn: parent

        property int clickPos: -1

        onClicked: {
            console.log("click")
            prefsPage.state = (prefsPage.state == "show") ? "hide" : "show"
        }

        onPressed: {
            clickPos = mouseX
        }

        onMousePositionChanged: {
            parent.state = 'changePage'
            parent.pos.x += mouseX - clickPos
        }

        onReleased: {
            if (parent.pos.x > (parent.width / 2)) {
                parent.state = 'treePage'
            }
            else if (parent.pos.x < -(parent.width / 2)) {
                parent.state = 'songPage'
            }
            else if (parent.state != "prefsPage")
                parent.state = 'mainPage'
        }
    }

    TreePage {
        id: treePage
    }
    SongPage {
        id: songPage
    }

    Rectangle {
        id: coverImage
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            margins: 40
        }
        width: parent.width - 40
        height: parent.width - 40
        color: "#770000"
    }

    Rectangle {
        id: slider
        width: parent.width
        height: 60
        color: "#0000aa"
        anchors {
            top: coverImage.bottom
            topMargin: 20
        }
        ProgressBar {
            width: parent.width
        }
    }

    Rectangle {
        id: toolbarPlayer
        width: parent.width
        color: "#00aa00"
        anchors {
            top: slider.bottom
            bottom: parent.bottom
            topMargin: 20
        }

        Row {
            width: parent.width
            height: parent.height
            Button {
                width: parent.width / 3
                height: parent.height
                ToolIcon {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                    iconId: "toolbar-mediacontrol-backwards"
                }
            }
            Button {
                width: parent.width / 3
                height: parent.height
                onClicked: { theme.inverted = !theme.inverted }
                ToolIcon {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                    iconId: "toolbar-mediacontrol-pause"
                }
            }
            Button {
                width: parent.width / 3
                height: parent.height
                ToolIcon {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                    iconId: "toolbar-mediacontrol-forward"
                }
            }
        }
    }

    Label {
        id: label
        text: "empty"
    }
    Label {
        anchors.top: label.bottom
        id: label1
        text: "empty"
    }
    Label {
        anchors.top: label1.bottom
        id: label2
        text: "empty2"
    }
    PrefsPage {
        id: prefsPage
    }
}
