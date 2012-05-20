import QtQuick 1.1
import com.nokia.meego 1.0
import "RplayView"

Page {
    id: prefsPage
    anchors.right: mainPage.left
    visible: true

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

    RplayView {
        id: prefsView
        dataTitle: "Settings"
        dataImage: "images/prefs.png"
        opacity: 1.0
    }
}
