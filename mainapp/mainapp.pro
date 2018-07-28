TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h


unix: !macx{
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

unix: macx{
INCLUDEPATH += /usr/local/include
LIBS += -L"/usr/local/lib" \
    -lopencv_world
}
