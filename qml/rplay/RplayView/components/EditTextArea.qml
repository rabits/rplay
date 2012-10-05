import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: editTextArea

    property alias text: editValue.text
    property alias myAnchors: editValue.anchors

    signal focused
    signal unfocused
    signal heightChanged

    property TextAreaStyle default_style: TextAreaStyle {}

    height: editValue.height

    TextArea {
        id: editValue
        width: parent.width
        //anchors.fill: parent
        textFormat: Text.PlainText
        font {
            pixelSize: 0
            pointSize: 18 * text_size
            family: "Monospace"
        }
        wrapMode: Text.Wrap
        style: TextAreaStyle {
            background: program_style.inverted ? default_style.background : "images/textedit-background.png"
            backgroundDisabled: program_style.inverted ? default_style.backgroundDisabled : "images/textedit-background-disabled.png"
            backgroundSelected: program_style.inverted ? default_style.backgroundSelected : "images/textedit-background-selected.png"
            backgroundError: program_style.inverted ? default_style.backgroundError : "images/textedit-background-error.png"
            textColor: program_style.inverted ? default_style.textColor : Qt.lighter(program_style.viewValueColor, text_bright);
            paddingTop: 3
            paddingBottom: 3
            selectionColor: program_style.inverted ? default_style.selectionColor : program_style.selectionColor
            selectedTextColor: program_style.inverted ? default_style.selectedTextColor : Qt.lighter(program_style.titleColor, text_bright);
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
