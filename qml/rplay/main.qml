import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    platformStyle: PageStackWindowStyle{ backgroundColor: "black" }

    initialPage: mainPage

    MainPage {
        id: mainPage
    }
}
