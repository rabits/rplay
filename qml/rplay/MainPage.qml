import QtQuick 1.1
import com.nokia.meego 1.0
import "RplayView"

Page {
    id: mainPage

    property variant component
    property variant sprite

    property string last_state: 'mainPage'

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
        id: fsView
        FsView {}
    }

    function setFolder(mypath) {
        if( sprite != null ) {
            sprite.destroy();
            component.destroy();
        }

        component = Qt.createComponent("RplayView.qml");
        sprite = component.createObject(mainPage,
                                        { dataPath: mypath
                                        , dataTitle: "Music Library"
                                        , dataType: 'folder'
                                        , dataImage: ctree.findCover(mypath)
                                        , view_delegate: fsView
                                        , view_model: ctree.treeContent(mypath)
                                        });
        sprite.start();
    }

    Component.onCompleted: {
        if( sprite == null ) {
            setFolder(ctree.parentDir(current_file))
        }
    }

    PrefsPage {
        id: prefsPage
    }
    SongPage {
        id: songPage
    }
}
