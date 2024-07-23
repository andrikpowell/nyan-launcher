QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += "APP_NAME=nyan-launcher"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    console.cpp \
    demodialog.cpp \
    endoom.cpp \
    historylist.cpp \
    main.cpp \
    mainwindow.cpp \
    settings.cpp \
    states.cpp

HEADERS += \
    console.h \
    demodialog.h \
    endoom.h \
    historylist.h \
    mainwindow.h \
    settings.h \
    states.h

FORMS += \
    console.ui \
    endoom.ui \
    historylist.ui \
    mainwindow.ui \
    settings.ui

win32 {
    CONFIG += static
    RC_ICONS = icons/nyan-launcher.ico
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static
}

mac {
    CONFIG += c++11 appbundle
    ICON = icons/nyan-launcher.icns
    !noport {
        APP_FILES.files = ./nyan-doom
        APP_FILES.files += ./nyan-doom.wad
    }
    deploy {
        QMAKE_APPLE_DEVICE_ARCHS=arm64 x86_64
        message("Compiling for both archs")
    }
    APP_FILES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += APP_FILES
    QMAKE_INFO_PLIST =./Info.plist
    HEADERS += Mac.h
    OBJECTIVE_SOURCES += Mac.mm
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
    build

DISTFILES += \
    Style.qml
