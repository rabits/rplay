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

        onMediaFullyLoaded: {
            update("");
        }

        onMetaDataChanged: {
            if( cplayer.fullyLoaded() )
                update("");
        }
    }
    Connections {
        target: clyrics

        onLyricsChanged: {
            update("");
        }
    }
    Connections {
        target: ccover

        onCoverChanged: {
            songView.dataImage = ccover.cover(ctree.parentDir(cplayer.currentFile()));
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
            songView.dataImage = ccover.cover(ctree.parentDir(cplayer.currentFile()));
            songView.dataPath = cplayer.currentFile();
        }

        if( (! clyrics.lyricsHas("") || cplayer.settingBool("preferences/network_get_lyrics_show_always"))
                && ! cplayer.settingBool("preferences/network_get_lyrics_only_on_songlyrics") )
            clyrics.lyricsNet(cplayer.artist(), cplayer.title());

        if( songView.dataType === "Metadata" ) {
            songView.view_model = cplayer.getMetaData();
            songView.dataNoItemsMessage = "Not found metadata";
        } else if( songView.dataType === "Extended Metadata" ) {
            songView.view_model = cplayer.getExtendedMetaData();
            songView.dataNoItemsMessage = "Not found extended metadata";
        } else {
            if( (! clyrics.lyricsHas("") || cplayer.settingBool("preferences/network_get_lyrics_show_always"))
                && cplayer.settingBool("preferences/network_get_lyrics_only_on_songlyrics") )
                clyrics.lyricsNet(cplayer.artist(), cplayer.title());
            songView.view_model = clyrics.lyrics("");
            songView.dataNoItemsMessage = 'Not found lyrics for<br/>"' + songView.dataTitle + '"';
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
        positionBoxUse: true
        playPauseOnCoverUse: true
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
