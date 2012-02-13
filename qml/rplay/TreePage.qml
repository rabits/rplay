import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: treePage
    visible: true
    anchors.right: mainPage.left

    ListView {
        anchors.fill: parent
        model: ListModel {
            ListElement { pic: "d"; name: "Melodic Death Metal" }
            ListElement { pic: "d"; name: "Death Metal" }
            ListElement { pic: "d"; name: "Brutal Death Metal" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "fffffffffffffffffffffileasdasdasdasdasdasdasdasd" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
            ListElement { pic: "f"; name: "file" }
        }
        delegate: Text {
            text: pic + " " + name
        }
    }
}
