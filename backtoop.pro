QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
VERSION = 0.1.0
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src/include/ src

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
#DEFINES += COMPILE_VID

win32 {
    TARGET = "BackToop"
    CONFIG(debug,debug|release){
        INCLUDEPATH += C:\VisualLeakDetector\include
        DEPENDPATH += C:\VisualLeakDetector\include
        if(contains(DEFINES, COMPILE_VID)) {
            LIBS += -LC:/VisualLeakDetector/lib/Win64 -lvld
            DEFINES += ENABLE_VID
        }

    }
}

win32-msvc* {
  QMAKE_CXXFLAGS += /utf-8
# use -j N in project settings when using jom instead
#  QMAKE_CXXFLAGS += /MP
}

SOURCES += \
    src/core/commoninclude.cpp \
    src/core/copyworker.cpp \
    src/core/main.cpp \
    src/gui/copyprogresswindow.cpp \
    src/gui/mainui.cpp \
    src/utils/calchash.cpp \
    src/utils/iconinstaller.cpp \
    src/utils/qssinstaller.cpp \
    src/utils/style/proxystyle.cpp \
    src/utils/watchfilemodel.cpp \
    src/utils/widget/lineeditex.cpp \
    src/utils/widget/messageboxexx.cpp \
    src/utils/widget/tablewidgetex.cpp \
    src/utils/widget/titlebar.cpp

HEADERS += \
    src/core/copyworker.h \
    src/gui/copyprogresswindow.h \
    src/include/commoninclude.h \
    src/gui/mainui.h \
    src/include/iconinstaller.h \
    src/include/qssinstaller.h \
    src/include/proxystyle.h \
    src/include/watchfilemodel.h \
    src/include/calchash.h \
    src/utils/widget/lineeditex.h \
    src/include/messageboxexx.h \
    src/utils/widget/tablewidgetex.h \
    src/utils/widget/titlebar.h

FORMS += \
    src/gui/copyprogresswindow.ui \
    src/gui/mainui.ui \
    src/utils/widget/messageboxexx.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/config/config.ini

RESOURCES += \
    src/resource/pic.qrc \
    src/resource/stylesheet.qrc
