QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


QT += webenginewidgets
QT += core gui network
QT += network
QT += core gui widgets charts

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    activitycalendarview.cpp \
    browserwindow.cpp \
    choosetime_style_widget.cpp \
    chooseweek_style_widget.cpp \
    custombutton.cpp \
    datecalendar.cpp \
    edit_class_button.cpp \
    editclasswidget.cpp \
    filehandler.cpp \
    linearprogressbar.cpp \
    main.cpp \
    mainwindow.cpp \
    savedata.cpp \
    smallcustombutton.cpp \
    smallwidget.cpp \
    subrepeat.cpp \
    tomatoclock.cpp \
    weatherfetcher.cpp \
    widget.cpp\
    deepseekclient.cpp

HEADERS += \
    activitycalendarview.h \
    browserwindow.h \
    choosetime_style_widget.h \
    chooseweek_style_widget.h \
    custombutton.h \
    datecalendar.h \
    deepseekclient.h \
    edit_class_button.h \
    editclasswidget.h \
    filehandler.h \
    header.h \
    linearprogressbar.h \
    mainwindow.h \
    savedata.h \
    smallcustombutton.h \
    smallwidget.h \
    subrepeat.h \
    tomatoclock.h \
    use_webengine.h \
    weatherfetcher.h \
    widget.h

FORMS += \
    choosetime_style_widget.ui \
    chooseweek_style_widget.ui \
    datecalendar.ui \
    editclasswidget.ui \
    mainwindow.ui \
    smallwidget.ui \
    subrepeat.ui \
    tomatoclock.ui \
    widget.ui

TRANSLATIONS += \
    qt_test_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \


RESOURCES += \
    res.qrc
