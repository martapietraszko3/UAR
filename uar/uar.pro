QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Generator.cpp \
    ModelARX.cpp \
    SerwisSymulacji.cpp \
    Testy_Generator.cpp \
    Testy_SS.cpp \
    Testy_UAR.cpp \
    UAR.cpp \
    arxokno.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    regulatorPID.cpp

HEADERS += \
    Generator.h \
    ModelARX.h \
    SerwisSymulacji.h \
    Testy_Generator.h \
    Testy_SS.h \
    Testy_UAR.h \
    UAR.h \
    arxokno.h \
    mainwindow.h \
    qcustomplot.h \
    regulatorPID.h

FORMS += \
    arxokno.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QT       += core gui printsupport

QMAKE_CXXFLAGS += -Wa,-mbig-obj
