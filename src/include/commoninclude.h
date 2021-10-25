#ifndef COMMONINCLUDE_H
#define COMMONINCLUDE_H

//#include <QWidget>
//#include "encryption/encrypted.h"
#include <QDateTime>

#define TO_STR1(I1) #I1
#define TO_STR2(I2) TO_STR1(I2)

#define TITLEBAR_TITLETEXT "BackToop"
#define TITLEBAR_TITLEICON ":/pic/BackToop.ico"
#define TITLEBAR_CLOSEICON ":/pic/close.png"

#define ABOUT_VERSION APP_VERSION
#define ABOUT_TIME  "2021.10.26"
#define ABOUT_BASE_QT "Qt " QT_VERSION_STR
#define ABOUT_BASE_CPP __cplusplus

#ifdef Q_CC_MSVC
#   define ABOUT_BASE_COMPILER_TYPE "MSVC "
#   define ABOUT_BASE_COMPILER _MSC_VER
#elif defined(Q_CC_GNU)
#   ifdef Q_CC_CLANG
#   define ABOUT_BASE_COMPILER_TYPE "GCC "
#   define ABOUT_BASE_COMPILER TO_STR2(__GNUC__) "." TO_STR2(__GNUC_MINOR__) "." TO_STR2(__GNUC_PATCHLEVEL__) "\n"\
    "Clang " TO_STR2(__clang_major__) "." TO_STR2(__clang_minor__)
#   define ABOUT_BASE_COMPILER_STRING
#else
#   define ABOUT_BASE_COMPILER_TYPE "GCC "
#   define ABOUT_BASE_COMPILER TO_STR2(__GNUC__) "." TO_STR2(__GNUC_MINOR__) "." TO_STR2(__GNUC_PATCHLEVEL__)
#   define ABOUT_BASE_COMPILER_STRING
#   endif
#elif defined(Q_CC_CLANG)
#   define ABOUT_BASE_COMPILER_TYPE "CLANG "
#   define ABOUT_BASE_COMPILER TO_STR2(__clang_major__) "." TO_STR2(__clang_minor__)
#   define ABOUT_BASE_COMPILER_STRING
#else
#   define ABOUT_BASE_COMPILER_TYPE "Unknown "
#   define ABOUT_BASE_COMPILER "  -  "
#endif

#if defined(Q_OS_LINUX)  && !defined(Q_OS_ANDROID)
#   define ABOUT_PLANTFORM "For Linux"
#elif defined(Q_OS_WINDOWS)
#   define ABOUT_PLANTFORM "For Windows"
#elif defined (Q_OS_ANDROID)
#   define ABOUT_PLANTFORM "For Android"
#else
#   define ABOUT_PLANTFORM "Unknown"
#   define TITLEBAR_TITLEICON ":/src/KeyContainer.ico"
#   define TITLEBAR_TITLETEXT "KeyContainer"
#endif




// ProxyStyle defines

#define TABBAR_LABEL_WIDTH             120
#define TABBAR_LABEL_HEIGHT             60
#define TABBAR_SHADOW_WIDTH              3
#define TABWIDGET_BUTTON_UP_HEIGHT      43                      // tabwidget menu的高度
#define TABBAR_BACKGROUND_COLOR               "#232323"     // tabwidget左侧选项卡的label的背景颜色，需要和qss里QTabWidget的背景颜色一致
#define TABWIDGET_MENU_BACKGROUND_COLOR       "#282828"     // 一横条的背景，宽度是button_up_height + offset
#define PUSHBUTTON_DISABLE_COLOR              "#282828"     // disable color 要比上边这个menu background color 浅
#define PUSHBUTTON_ON_WIDGET_BACKGROUND_COLOR "#333333"     // 设置在widget上而不在一横条背景上的PushButton在普通状态时的背景颜色
#define CHECKBOX_DIABLE_COLOR                 "#808080"     // QCheckBox disabled color
#define CHECKBOX_CHECK_WIDTH             4

#define TABWIDGET_MENU_HORIZONTAL_MARGIN 0     // 一横条的高与pushbutton的高度差，要比pushbutton宽。
#define TABWIDGET_MENU_VERTICAL_MARGIN   5     // 一横条的高与pushbutton的高度差，要比pushbutton宽。

#define PADDING_UP    4
#define PADDING_LEFT  4
#define PADDING_DOWN  4

#define KEYMAP_JSON_ENGINE_VERSION "1.0"

#endif // COMMONINCLUDE_H
