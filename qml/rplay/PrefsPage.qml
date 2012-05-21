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
        text: cplayer.setting("preferences/music_library_path")
    }

    Button {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text: "It's prefs"
        onClicked: { screen.allowSwipe = !screen.allowSwipe }
    }

    function saveSettings() {
        cplayer.setting("preferences/music_library_path", s_root_music.text);
    }

    Component {
        id: prefsDelegate
        PrefsDelegate {}
    }

    RplayView {
        id: prefsView
        dataTitle: "Settings"
        dataImage: "images/prefs.png"
        view_model: cplayer.prefsContent()
        view_delegate: prefsDelegate
        opacity: 1.0
    }
}
