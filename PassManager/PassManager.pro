QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    accountpageform.cpp \
    createaccountform.cpp \
    creategroupform.cpp \
    editgrouppageform.cpp \
    groupaccountform.cpp \
    grouppageform.cpp \
    itemblock.cpp \
    main.cpp \
    mainwindow.cpp \
    popup.cpp \
    windowframe.cpp

HEADERS += \
    account_info.h \
    accountpageform.h \
    createaccountform.h \
    creategroupform.h \
    editgrouppageform.h \
    group_info.h \
    groupaccountform.h \
    grouppageform.h \
    itemblock.h \
    mainwindow.h \
    popup.h \
    types.h \
    windowframe.h

FORMS += \
    accountpageform.ui \
    createaccountform.ui \
    creategroupform.ui \
    editgrouppageform.ui \
    groupaccountform.ui \
    grouppageform.ui \
    itemblock.ui \
    mainwindow.ui \
    windowframe.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc

# win32: RC_ICONS = $$PWD/resources/icons/icon.ico
