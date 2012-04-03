import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: treePage
    visible: true
    anchors.right: mainPage.left

    Component {
        id: delegateItem
        Item {
            id: listItem

            signal clicked
            property alias pressed: mouseArea.pressed
            property int header: Math.max(picture.height, info.height) + 8

            height: container.height
            width: parent.width

            Rectangle {
                id: container
                color: "black"

                height: Math.max(picture.height, info.height) + 8 + additional.height + border.height
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

                Rectangle {
                    id: info
                    color: parent.color
                    height: title.height + type.height
                    anchors {
                        left: picture.right
                        right: parent.right
                        topMargin: 4
                        leftMargin: 5
                    }

                    Label {
                        id: title
                        anchors {
                            left: parent.left
                            right: parent.right
                            leftMargin: 5
                        }
                        text: model.title
                        font.weight: Font.Bold
                        font.pointSize: 20
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
                        font.pointSize: 14
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
                        font.pointSize: 14
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
                        font.pointSize: 10
                        color: "#444"
                    }
                }
                Rectangle {
                    id: additional
                    color: parent.color
                    clip: true
                    opacity: 0.0
                    height: 0
                    anchors {
                        top: (picture.height > info.height) ? picture.bottom : info.bottom
                        left: parent.left
                        right: parent.right
                        leftMargin: 20
                    }

                    ListView {
                        id: subList

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
                        if( model.type != 'file' ) {
                            listItem.clicked();
                            listItem.parent.parent.interactive = (listItem.parent.parent.interactive) ? false : true
                            listItem.state = (listItem.state === 'enlarged') ? '' : 'enlarged'
                            if( listItem.state === 'enlarged' ) {
                                subList.model = ctree.treeContent(model.path)
                            }
                        } else {
                            cplayer.playFile(model.path)
                        }
                    }
                }
            }

            states: [
                State {
                    name: "enlarged"
                    PropertyChanges { target: picture; height: 150; width: 150 }
                    PropertyChanges { target: title; elide: Text.ElideNone }
                    PropertyChanges { target: listItem.parent.parent; contentY: listItem.y }
                    PropertyChanges { target: additional; height: listItem.parent.parent.height - Math.max(picture.height, info.height); opacity: 1.0 }
                    PropertyChanges { target: path; visible: true }
                }
            ]
            transitions: [
                Transition {
                    to: "enlarged"
                    SequentialAnimation {
                        PropertyAnimation { properties: "visible, elide"; duration: 0 }
                        PropertyAnimation { properties: "height, width, visible, opacity"; duration: 200 }
                        PropertyAnimation { properties: "contentY"; duration: 100 }
                    }

                },
                Transition {
                    from: "enlarged"
                    SequentialAnimation {
                        PropertyAnimation { property: "contentY"; duration: 100 }
                        PropertyAnimation { properties: "height, width, visible, opacity"; duration: 200 }
                        PropertyAnimation { properties: "visible, elide"; duration: 0 }
                    }

                }
            ]
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "black"

        ListView {
            id: rootList
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
}
