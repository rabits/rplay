import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    id: playPositionBox

    property bool useChangePosition: false

    gradient: Gradient {
        GradientStop { position: 0.0; color: "transparent" }
        GradientStop { position: 0.1; color: "#66444444" }
        GradientStop { position: 0.9; color: "#66aaaaaa" }
        GradientStop { position: 1.0; color: "transparent" }
    }

    radius: 2.0

    function two(x) {return ((x>9)?"":"0")+x}
    function msec2time(msec) {
        msec = Math.floor(msec / 1000)
        var time = "00:00";

        if( msec > 0 )
        {
            time = two(msec % 60);

            msec = Math.floor(msec / 60)
            time = two(msec % 60) + ":" + time;

            msec = Math.floor(msec / 60)
            if( msec > 0 )
                time = (msec % 24) + ":" + time;
        }

        return time;
    }

    Label {
        id: currentTime
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 5
        }
        font.pixelSize: parent.height - 4
        color: Qt.lighter("#999", text_bright);
    }

    Label {
        id: durationTime
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 5
        }
        font.pixelSize: parent.height - 4
        color: Qt.lighter("#999", text_bright);
    }

    Rectangle {
        id: playPosition
        color: "transparent"
        anchors {
            fill: parent
            rightMargin: parent.width
        }

        gradient: Gradient {
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 0.1; color: "transparent" }
            GradientStop { position: 0.42; color: colorGlow }
            GradientStop { position: 0.58; color: colorGlow }
            GradientStop { position: 0.9; color: "transparent" }
            GradientStop { position: 1.0; color: "transparent" }
        }
    }

    Timer {
        interval: 200
        running: playPositionBox.visible
        repeat: true

        onTriggered: {
            playPosition.anchors.rightMargin = parent.width - (parent.width * (cplayer.currentFilePosition() / cplayer.currentFileDuration()));
            currentTime.text = parent.msec2time(cplayer.currentFilePosition());
            durationTime.text = parent.msec2time(cplayer.currentFileDuration());
        }
    }

    MouseArea {
        visible: useChangePosition
        anchors.fill: parent
        onPressed: {
            if( cplayer.currentFileDuration() > 0 )
                cplayer.currentFilePosition(cplayer.currentFileDuration() / parent.width * mouseX);
            else
                showBanner("Can't change position - please, play the file", "", "");
        }
    }
}
