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
            update("");
        }
    }

    Connections {
        target: platformWindow

        onViewModeChanged: {
            if( platformWindow.viewMode == WindowState.Fullsize ) {
                songView.titleSize = 16
                songView.rotation = 0
                songView.anchors.fill = songPage
            } else {
                songView.titleSize = 30
                songView.rotation = 90
                songView.anchors.fill = null
                songView.anchors.centerIn = songPage
                songView.height = mainPage.width - 70
                songView.width = mainPage.height
            }
        }
    }

    function update(type) {
        if( type !== "" )
            songView.dataType = type;
        else {
            songView.dataTitle = ctree.getName(cplayer.currentFile());
            songView.dataImage = ctree.findCover(ctree.parentDir(cplayer.currentFile()));
            songView.dataPath = cplayer.currentFile();
        }

        if( songView.dataType === "Metadata" ) {
            songView.view_model = cplayer.getMetaData();
        } else if( songView.dataType === "Extended Metadata" ) {
            songView.view_model = cplayer.getExtendedMetaData();
        } else {
            songView.view_model = cplayer.getLyrics("");
        }
    }

    Component {
        id: keyvalueDelegate
        KeyValueDelegate {}
    }

    RplayView {
        id: songView
        dataTitle: "Song"
        dataType: "Metadata"
        dataImage: "images/album.png"
        view_delegate: keyvalueDelegate
        opacity: 1.0
        onClicked: {
            if( songView.dataType === "Metadata" ) {
                update("Extended Metadata");
            } else if( songView.dataType === "Extended Metadata" ) {
                update("Lyrics");
            } else {
                update("Metadata");
            }
        }
    }
}
