QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clickablelabel.cpp \
    customlineedit.cpp \
    database.cpp \
    dotd.cpp \
    interfacechooser.cpp \
    main.cpp \
    ripplebutton.cpp \
    startup.cpp \
    cardwidget.cpp \
    cafeinterface.cpp \
    add.cpp \
    delete.cpp \
    update.cpp \
    userinterface.cpp \

HEADERS += \
    clickablelabel.h \
    customlineedit.h \
    database.h \
    dotd.h \
    interfacechooser.h \
    ripplebutton.h \
    startup.h \
    cardwidget.h \
    cafeinterface.h \
    add.h \
    delete.h \
    update.h \
    userinterface.h \

FORMS += \
    cafeinterface.ui \
    cardwidget.ui \
    delete.ui \
    interfacechooser.ui \
    add.ui \
    startup.ui \
    update.ui \
    userinterface.ui \
    dotd.ui

RESOURCES += Resources.qrc

DISTFILES += data/cafe.db

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
