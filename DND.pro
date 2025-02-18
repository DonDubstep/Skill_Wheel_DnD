QT       += core gui widgets
#QT       -= network sql xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#QMAKE_CXXFLAGS_RELEASE += -s
CONFIG += c++11

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
    description.cpp \
    main.cpp \
    mainwindow.cpp \
    skill.cpp

HEADERS += \
    description.h \
    mainwindow.h \
    settings.h \
    skill.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
win64
{

    DESTDIR = $$PWD/bin
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
