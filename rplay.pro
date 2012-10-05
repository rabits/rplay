# Version
VERSION='1.2.4'
VERSTR='\\"$${VERSION}\\"'
DEFINES += PROJECT_VERSION=\"$${VERSTR}\"

# Add more folders to ship with the application, here
folder_01.source = qml/rplay
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian {
    # Supports Symbian^3 by default and also S60 5.0 if built against any of those.
    platform_product_id = S60ProductID
    platform_product_id = $$addLanguageDependentPkgItem(platform_product_id)
    pkg_platform_dependencies = \
        "; Default HW/platform dependencies" \
        "[0x20022E6D],0,0,0,{$$platform_product_id}"
    contains(S60_VERSION, 5.0) {
        pkg_platform_dependencies += \
            "[0x1028315F],0,0,0,{$$platform_product_id}"
    }

    TARGET.UID3 = 0xE24A1DB5
    # Allow network access on Symbian
    TARGET.CAPABILITY += NetworkServices
    CONFIG += qt-components
}

QT += network

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
CONFIG += mobility
MOBILITY += multimedia

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable meegotouch
PKGCONFIG += qdeclarative-boostable qmsystem2

# Attach flite libs
exists($$PWD/lib/flite/lib) {
    PKGCONFIG += alsa
    LIBS += -L$$PWD/lib/flite/lib -lflite_cmu_us_slt.shared -lflite_usenglish.shared -lflite_cmulex.shared -lflite.shared
    INCLUDEPATH += $$PWD/lib/flite/include
    DEFINES += USE_VOICE
}

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    src/ctree.cpp \
    src/listmodel.cpp \
    src/cplayer.cpp \
    src/model/ctreeitem.cpp \
    src/model/ckeyvalueitem.cpp \
    src/clyrics.cpp \
    src/ccover.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/rplay/MainPage.qml \
    i18n/tr_ru.ts \
    i18n/tr_en.ts

HEADERS += \
    src/ctree.h \
    src/listmodel.h \
    src/cplayer.h \
    src/model/ctreeitem.h \
    src/model/ckeyvalueitem.h \
    src/clyrics.h \
    src/ccover.h

contains(DEFINES, USE_VOICE) {
    HEADERS += src/cvoice.h
    SOURCES += src/cvoice.cpp
}

RESOURCES += \
    resources.qrc
