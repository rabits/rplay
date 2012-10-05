import QtQuick 1.0

Item {
    property bool inverted: false

    // Common
    property color  foregroundColor: inverted ? "#000" : "#fff"
    property color  backgroundColor: inverted ? "#fff" : "#000"
    property color  backgroundGradientColor: inverted ? "#fafafa" : "#050505"
    property string backgroundImage: inverted ? "images/rabit_inverted.png" : "images/rabit.png"

    property color titleRectColor: inverted ? "#11000000" : "#11ffffff"
    property color borderColor: inverted ? "#bbb" : "#444"
    property color endListBorderColor: inverted ? "#000" : "#fff"

    property color playposStartColor: inverted ? "#66bbbbbb" : "#66444444"
    property color playposEndColor: inverted ? "#66555555" : "#66aaaaaa"

    property color infoDialogColor: inverted ? "#ccffffff" : "#cc000000"

    property color selectionColor: inverted ? "#55000000" : "#55ffffff"

    // Text Color
    property color titleColor: inverted ? "#777" : "#888"
    property color typeColor: inverted ? "#aaa" : "#555"
    property color insideColor: inverted ? "#aaa" : "#555"
    property color pathColor: inverted ? "#888" : "#777"
    property color viewValueColor: inverted ? "#666" : "#999"
    property color noItemsColor: inverted ? "#55000000" : "#55ffffff"
    property color scrollColor: inverted ? "#aaa" : "#555"
    property color titleDataColor: inverted ? "#222" : "#ddd"
    property color contentDataColor: inverted ? "#444" : "#aaa"
}
