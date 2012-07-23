import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: editTextArea

    property alias text: editValue.text
    property alias myAnchors: editValue.anchors

    signal focused
    signal unfocused
    signal heightChanged

    height: editValue.height

    TextArea {
        id: editValue
        width: parent.width
        //anchors.fill: parent
        textFormat: Text.PlainText
        font {
            pixelSize: 0
            pointSize: 18
            family: "Monospace"
        }
        wrapMode: Text.Wrap
        style: TextAreaStyle {
            background: "images/textedit-background.png"
            backgroundDisabled: "images/textedit-background-disabled.png"
            backgroundSelected: "images/textedit-background-selected.png"
            backgroundError: "images/textedit-background-error.png"
            textColor: Qt.lighter("#666", text_bright);
            paddingTop: 3
            paddingBottom: 3
            selectionColor: "#55ffffff"
            selectedTextColor: Qt.lighter("#888", text_bright);
        }

        onActiveFocusChanged: {
            if( activeFocus )
                focused();
            else
            {
                closeSoftwareInputPanel();
                unfocused();
            }
        }
    }
}
