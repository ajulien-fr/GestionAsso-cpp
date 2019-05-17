#-------------------------------------------------
#
# Project created by QtCreator 2019-05-10T01:32:36
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GestionAsso
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        main-window.cpp \
        menu-bar.cpp \
        database.cpp \
        message-box.cpp \
        table-view.cpp \
        pages/accueil-page.cpp \
        pages/actions-page.cpp \
        pages/animaux-page.cpp \
        pages/bilans-page.cpp \
        pages/comptes-page.cpp \
        pages/intervenants-page.cpp \
        pages/objets-page.cpp \
        widgets/action.cpp \
        widgets/animal.cpp \
        widgets/bilan.cpp \
        widgets/compte.cpp \
        widgets/intervenant.cpp \
        widgets/objet.cpp

HEADERS += \
        main-window.h \
        database.h \
        message-box.h \
        table-view.h \
        pages/accueil-page.h \
        pages/actions-page.h \
        pages/animaux-page.h \
        pages/bilans-page.h \
        pages/comptes-page.h \
        pages/intervenants-page.h \
        pages/objets-page.h \
        widgets/action.h \
        widgets/animal.h \
        widgets/bilan.h \
        widgets/compte.h \
        widgets/intervenant.h \
        widgets/objet.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    auteur.txt
