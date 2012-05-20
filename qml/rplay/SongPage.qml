import QtQuick 1.1
import com.nokia.meego 1.0
import "RplayView"

Page {
    id: songPage
    visible: true
    anchors.left: mainPage.right

    RplayView {
        id: songView
        dataTitle: "Song"
        dataImage: "images/album.png"
        opacity: 1.0
    }
}
