import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: mainPage

    property variant component
    property variant sprite

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

    function setFolder(mypath) {
        if( sprite != null ) {
            sprite.destroy();
            component.destroy();
        }

        component = Qt.createComponent("DirectoryView.qml");
        sprite = component.createObject(mainPage, {
                                            dataPath: mypath,
                                            dataImage: ctree.findCover(mypath)
                                        });
        sprite.start();
    }

    Component.onCompleted: {
        setFolder(ctree.parentDir(current_file))
    }

    PrefsPage {
        id: prefsPage
    }
    SongPage {
        id: songPage
    }
}
