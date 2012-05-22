import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "RplayView"

Page {
    id: songPage
    visible: true
    anchors.left: mainPage.right

    Connections {
        target: cplayer

        onMetaDataChanged: {
            update();
        }
    }

    function update() {
        songView.dataTitle = ctree.getName(cplayer.currentFile())
        songView.dataImage = ctree.findCover(ctree.parentDir(cplayer.currentFile()))
        songView.view_model = cplayer.getMetaData();
        songView.dataType = "Metadata"
    }

    Component {
        id: keyvalueDelegate
        KeyValueDelegate {}
    }

    RplayView {
        id: songView
        dataTitle: "Song"
        dataType: "no metadata"
        dataImage: "images/album.png"
        view_delegate: keyvalueDelegate
        opacity: 1.0
        onClicked: {
            if( songView.dataType === "Metadata" ) {
                songView.view_model = cplayer.getExtendedMetaData();
                songView.dataType = "Extended Metadata"
            } else {
                update()
            }
        }
    }
}
