import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "RplayView"

Page {
    id: mainPage

    property variant component
    property variant sprite

    property string last_state: 'mainPage'

    property string previousCurrentParent: ctree.parentDir(cplayer.currentFile())

    function switch_page(leftright) {
        if( leftright < 0 ) {
            if( last_state === 'mainPage' )
                mainPage.state = 'songPage'
            else if( last_state === 'prefsPage' )
                mainPage.state = 'mainPage'
            else
                mainPage.state = 'songPage'
        } else if( leftright > 0 ) {
            if( last_state === 'mainPage' )
                mainPage.state = 'prefsPage'
            else if( last_state === 'songPage' )
                mainPage.state = 'mainPage'
            else
                mainPage.state = 'prefsPage'
        } else
            mainPage.state = last_state
        last_state = mainPage.state
    }

    states: [
        State {
            name: "mainPage"
            PropertyChanges{ target: mainPage; pos.x: 0 }
        },
        State {
            name: "prefsPage"
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

    /*SequentialAnimation {
        id: toMainPage
        NumberAnimation { property: "pos.x"; to: 0; duration: 100 }
    }*/

    Component {
        id: treeDelegate
        TreeDelegate {}
    }

    function setFolder(mypath) {
        if( sprite != null ) {
            if( ! mypath )
                mypath = ctree.parentDir(sprite.dataPath);
            sprite.destroy();
            component.destroy();
        }

        component = Qt.createComponent("RplayView/RplayView.qml");
        sprite = component.createObject(mainPage,
                                        { dataPath: mypath
                                        , dataTitle: mypath === "" ? "Music Library" : ctree.getName(mypath)
                                        , dataType: 'folder'
                                        , dataImage: ctree.findCover(mypath)
                                        , view_delegate: treeDelegate
                                        , view_model: ctree.treeContent(mypath)
                                        });
        sprite.clicked.connect(setFolder);
        sprite.start();
    }

    Component.onCompleted: {
        if( sprite == null ) {
            setFolder(ctree.parentDir(cplayer.currentFile()))
        }
    }

    Connections {
        target: platformWindow

        onViewModeChanged: {
            if( platformWindow.viewMode == WindowState.Fullsize ) {
                mainPage.state = last_state
            } else {
                mainPage.state = 'songPage'
            }
        }
    }

    Connections {
        target: cplayer

        onNext: {
            if( (previousCurrentParent !== ctree.parentDir(cplayer.currentFile())) && (previousCurrentParent === sprite.dataPath) )
            {
                previousCurrentParent = ctree.parentDir(cplayer.currentFile());
                setFolder(previousCurrentParent);
            }
        }
    }

    PrefsPage {
        id: prefsPage
    }
    SongPage {
        id: songPage
    }
}
