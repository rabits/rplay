import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: prefsPage
    state: "hide"

    states: [
        State {
            name: "show"
            PropertyChanges{ target: prefsPage; opacity: 0.9; visible: true }
        },
        State {
            name: "hide"
            PropertyChanges{ target: prefsPage; opacity: 0.0; visible: false }
        }
    ]

    transitions: [
        Transition {
            from: "hide"
            SequentialAnimation {
                PropertyAnimation { properties: "visible" }
                NumberAnimation { properties: "opacity"; duration: 100 }
            }
        },
        Transition {
            from: "show"
            SequentialAnimation {
                NumberAnimation { properties: "opacity"; duration: 100 }
                PropertyAnimation { properties: "visible" }
            }
        }
    ]

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    TextField {
        id: s_root_music
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        text: cplayer.setting("ctree/root_music")
    }

    Button {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text: "It's prefs"
        onClicked: { screen.allowSwipe = !screen.allowSwipe }
    }

    function saveSettings() {
        cplayer.setting("ctree/root_music", s_root_music.text);
    }
}
