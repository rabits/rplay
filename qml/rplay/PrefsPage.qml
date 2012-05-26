import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "RplayView"

Page {
    id: prefsPage
    anchors.right: mainPage.left
    visible: true

    Component {
        id: keyvalueDelegate
        KeyValueDelegate {}
    }

    RplayView {
        id: prefsView
        dataTitle: "Settings"
        dataImage: "images/prefs.png"
        view_model: cplayer.prefsContent()
        view_delegate: keyvalueDelegate
        opacity: 1.0
    }
}
