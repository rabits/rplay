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

    Connections {
        target: cplayer

        onSettingChanged: {
            if( key === "preferences/repeat_shuffle_folder" )
            {
                prefsView.view_model.destroy();
                prefsView.view_model = cplayer.prefsContent();
            }
        }
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
