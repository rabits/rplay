import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: mainPage

    orientationLock: PageOrientation.LockPortrait

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

    Component {
        id: delegateItem
        Item {
            id: listItem
            objectName: model.path

            signal clicked
            property alias pressed: mouseArea.pressed
            property variant parent_list: parent.parent
            property variant parent_item: parent_list.parent.parent.parent
            property int head_height: Math.max(picture.height, info.height) + 8
            property color active: (current_file_array[model.level] == model.title) ? "#55ffffff" : "transparent"

            height: container.height
            width: parent.width

            Rectangle {
                id: container
                color: "transparent"

                height: listItem.head_height + additional.height + border.height
                width: parent.width

                Image {
                    id: picture
                    anchors {
                        top: parent.top
                        left: parent.left
                        topMargin: 4
                    }

                    width: 50
                    height: 50
                    source: model.picture ? model.picture : (model.type === 'album' ? "images/album.png" : ( model.type === 'folder' ? "images/folder.png" : "images/file.png") )
                    fillMode: Image.PreserveAspectFit
                }

                Item {
                    id: info
                    height: title.height + type.height
                    anchors {
                        left: picture.right
                        right: parent.right
                        topMargin: 4
                        leftMargin: 5
                        bottomMargin: 4
                    }

                    Label {
                        id: title
                        anchors {
                            left: parent.left
                            right: parent.right
                            leftMargin: 5
                        }
                        text: model.title
                        font {
                            weight: Font.Bold
                            pixelSize: 0
                            pointSize: 16
                        }
                        color: "gray"
                        elide: Text.ElideRight
                    }

                    Label {
                        id: type
                        anchors {
                            top: title.bottom
                            left: parent.left
                            leftMargin: 10
                        }
                        text: model.type
                        font {
                            pixelSize: 0
                            pointSize: 12
                        }
                        color: "#444"
                    }

                    Label {
                        id: inside
                        visible: model.inside ? true : false
                        anchors {
                            top: title.bottom
                            right: parent.right
                            rightMargin: 10
                        }
                        text: model.inside ? model.inside : ""
                        font {
                            pixelSize: 0
                            pointSize: 12
                        }
                        color: "#444"
                    }

                    Label {
                        id: path
                        visible: false
                        anchors {
                            top: type.bottom
                            left: parent.left
                            right: parent.right
                            leftMargin: 10
                        }
                        text: model.path
                        font {
                            pixelSize: 0
                            pointSize: 10
                        }
                        color: "#777"
                    }
                }

                Rectangle {
                    id: header
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: additional.top
                        bottomMargin: -4
                    }
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "transparent" }
                        GradientStop { position: 0.05; color: listItem.active }
                        GradientStop { position: 0.3; color: "transparent" }
                        GradientStop { position: 0.7; color: "transparent" }
                        GradientStop { position: 0.95; color: listItem.active }
                        GradientStop { position: 1.0; color: "transparent" }
                    }
                }

                Rectangle {
                    id: additional
                    color: "transparent"
                    clip: true
                    opacity: 0.0
                    height: 0
                    anchors {
                        top: (picture.height > info.height) ? picture.bottom : info.bottom
                        left: parent.left
                        right: parent.right
                        topMargin: 4
                        leftMargin: (model.level < 7) ? 5 : 0
                    }

                    ListView {
                        id: subList
                        objectName: "subList"

                        anchors.fill: parent
                        delegate: delegateItem
                        ScrollDecorator {
                            flickableItem: parent
                        }
                    }
                }

                Rectangle {
                    radius: 5.0
                    anchors.fill: parent
                    color: "white"
                    opacity: 0.1
                    visible: mouseArea.pressed
                }

                Rectangle {
                    id: border
                    height: 1
                    color: "#444"
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                }

                MouseArea {
                    id: mouseArea;
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: additional.top
                    }

                    onClicked: {
                        if( model.type !== 'file' ) {
                            var do_state = 'enlarged'
                            if( model.level < 2 )
                                do_state = 'enlargedRoot'
                            listItem.clicked();
                            listItem.parent_list.interactive = (listItem.parent_list.interactive) ? false : true
                            listItem.state = (listItem.state === do_state) ? '' : do_state
                            if( listItem.state === do_state ) {
                                subList.model = ctree.treeContent(model.path)
                            }
                        } else {
                            cplayer.playFile(model.path)
                        }
                    }

                    /*property int pressPosX
                    property int pressPosY

                    property bool vertical_swipe: 0
                    property bool horizontal_swipe: 0

                    //onClicked: {
                    //    console.log("click")
                    //    if( prefsPage.state == "show" )
                    //    {
                    //        prefsPage.saveSettings()
                    //        redrawTree()
                    //    }
                    //    prefsPage.state = (prefsPage.state == "show") ? "hide" : "show"
                    //}

                    onPressed: {
                        pressPosX = mouseX
                        pressPosY = mouseY
                    }

                    onMousePositionChanged: {
                        pressPosX =
                        pressPosY = mouseY

                        if( vertical_swipe ) {
                            mainPage.pos.x += mouseX - clickPos
                        }
                        else if( ! (horizontal_swipe || vertical_swipe) ) {
                            if( pressPosX - mouseX > vertical_swipe + 20 ) {
                                horizontal_swipe = true
                            }
                            else if( vertical_swipe > horizontal_swipe + 20 ) {
                                vertical_swipe = true
                                mainPage.state = 'changePage'
                                listItem.parent_list.interactive = false
                            }
                        }
                    }

                    onReleased: {
                        if (mainPage.pos.x > (mainPage.width / 2)) {
                            mainPage.state = 'prefsPage'
                        }
                        else if (mainPage.pos.x < -(mainPage.width / 2)) {
                            mainPage.state = 'songPage'
                        }
                        else if (mainPage.state != "prefsPage")
                            mainPage.state = 'mainPage'

                        if (vertical_swipe) {
                            listItem.parent_list.interactive = true
                            vertical_swipe = false
                        }
                        horizontal_swipe = false
                    }*/
                }
            }

            states: [
                State {
                    name: "enlargedRoot"
                    PropertyChanges { target: picture; height: 150; width: 150 }
                    PropertyChanges { target: listItem.parent_list; contentY: listItem.y }
                    PropertyChanges { target: additional; height: listItem.parent_list.height - listItem.head_height; opacity: 1.0 }
                    PropertyChanges { target: path; visible: true }
                },
                State {
                    name: "enlarged"
                    extend: "enlargedRoot"
                    PropertyChanges {
                        target: listItem.parent_item.parent_list;
                        contentY: listItem.parent_item.y + listItem.parent_item.head_height + 1
                    }
                    PropertyChanges { target: listItem.parent_list.parent; height: mainPage.height + 1 }
                }
            ]
            transitions: [
                Transition {
                    from: ""
                    to: "enlarged"
                    reversible: true
                    SequentialAnimation {
                        PropertyAnimation { properties: "visible, elide"; duration: 0 }
                        PropertyAnimation { properties: "height, width, visible, opacity"; duration: 200 }
                        PropertyAnimation { properties: "contentY"; duration: 100 }
                    }
                },
                Transition {
                    from: ""
                    to: "enlargedRoot"
                    reversible: true
                    SequentialAnimation {
                        PropertyAnimation { properties: "visible, elide"; duration: 0 }
                        PropertyAnimation { properties: "height, width, visible, opacity"; duration: 200 }
                        PropertyAnimation { properties: "contentY"; duration: 100 }
                    }
                }
            ]
        }
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "black" }
            GradientStop { position: 0.98; color: "#050505" }
            GradientStop { position: 1.0; color: "black" }
        }

        ListView {
            id: rootList
            objectName: "rootList"
            anchors.fill: parent
            delegate: delegateItem
            model: ctree.treeContent("")

            ScrollDecorator {
                flickableItem: parent
            }
        }
    }

    function redrawTree() {
        rootList.model = ctree.treeContent("")
    }

    PrefsPage {
        id: prefsPage
    }
    SongPage {
        id: songPage
    }
}
