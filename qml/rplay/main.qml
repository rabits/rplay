import QtQuick 1.1
import com.nokia.meego 1.0
import "RplayView/dialog"
import com.nokia.extras 1.0

PageStackWindow {
    id: appWindow
    style: PageStackWindowStyle{ backgroundColor: program_style.backgroundColor }

    initialPage: mainPage

    property string text_bright: cplayer.setting("preferences/text_bright")
    property string text_size: cplayer.setting("preferences/text_size")
    property ProgramStyle program_style: ProgramStyle { inverted: cplayer.setting("preferences/style_inverse") }

    MainPage {
        id: mainPage
    }

    function showInfo(title, text) {
        textDialog.titleText = title;
        textDialog.contentText = text;
        textDialog.show();
    }

    function showBanner(text, wait, image) {
        infoBanner.text = text;
        infoBanner.timerShowTime = wait !== "" ? wait : 3000;
        infoBanner.iconSource = image !== "" ? image : "image://theme/icon-m-common-yellow"
        infoBanner.show();
    }

    TextDialog {
        id: textDialog
        anchors.topMargin: 40
    }

    InfoBanner {
        id: infoBanner
        topMargin: 40
    }

    Component.onCompleted: {
        if( cplayer.settingBool("rplay/show_help_once") )
        {
            showInfo(qsTr("About and Help"), cplayer.helpContent());
            cplayer.setting("rplay/show_help_once", false);
        }
    }
}
