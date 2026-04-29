QT       += core gui widgets
#QT       -= network sql xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#QMAKE_CXXFLAGS_RELEASE += -s
CONFIG += c++11
RC_ICONS = src/skill_wheel_icon.ico
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    description/description.cpp \
    headerwidget/header_selection/header_selection.cpp \
    headerwidget/headerwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    messagebox/messagebox.cpp \
    pagewidget/pagewidget.cpp \
    pagewidget/selection/selection.cpp \
    preset_handler/preset_handler.cpp \
    skill/skill.cpp

HEADERS += \
    description/description.h \
    headerwidget/header_selection/header_selection.h \
    headerwidget/headerwidget.h \
    mainwindow.h \
    messagebox/messagebox.h \
    pagewidget/pagewidget.h \
    pagewidget/selection/selection.h \
    preset_handler/preset_handler.h \
    settings.h \
    skill/skill.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += \
    $$PWD/description \
    $$PWD/headerwidget \
    $$PWD/headerwidget/header_selection \
    $$PWD/pagewidget \
    $$PWD/pagewidget/selection \
    $$PWD/skill \
    $$PWD/preset_handler \
    $$PWD/messagebox \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/Skill_Wheel_app
else: unix:!android: target.path = /opt/$${TARGET}/Skill_Wheel_app
!isEmpty(target.path): INSTALLS += target
win64
{

    DESTDIR = $$PWD/Skill_Wheel_app
    QMAKE_POST_LINK = windeployqt $$shell_path($$DESTDIR/$${TARGET}.exe)

#    message($$shell_path($$DESTDIR/$${TARGET}.exe))
    # Copy data folder
    SOURCE_DIR = $$PWD/src
    TARGET_DIR = $$DESTDIR/src

    copydata.commands = if not exist $$shell_path($$TARGET_DIR) mkdir $$shell_path($$TARGET_DIR) && xcopy /E /I /Y $$shell_path($$SOURCE_DIR) $$shell_path($$TARGET_DIR)
    QMAKE_EXTRA_TARGETS += copydata
    PRE_TARGETDEPS += copydata
}

DISTFILES += \
    README.md \
    src/data.json
