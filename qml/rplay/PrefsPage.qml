import ListModels 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "RplayView"
import "RplayView/dialog"

Page {
    id: prefsPage
    anchors.right: mainPage.left
    visible: true

    Component {
        id: keyvalueDelegate
        KeyValueDelegate {}
    }

    FsDialog {
        id: selectFolderDialog
        titleText: "Select folder"
        filterList: [""]
    }
    Component {
        id: textEditDialog
        Dialog {}
    }
    Component {
        id: numberEditDialog
        Dialog {}
    }
    Component {
        id: boolEditDialog
        Dialog {}
    }

    property alias selectedFolder: selectFolderDialog.selectedFile

    function selectFolder(path, func) {
        selectFolderDialog.currentFolder = path;
        selectFolderDialog.accepted.connect(func)
        selectFolderDialog.open()
    }

    RplayView {
        id: prefsView
        dataTitle: "Settings"
        dataImage: "images/prefs.png"
        view_model: cplayer.prefsContent()
        view_delegate: keyvalueDelegate
        opacity: 1.0
    }
}
