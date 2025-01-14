TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        admin.c \
        fileoperations.c \
        main.c \
        mainmenu.c \
        user.c

HEADERS += \
    admin.h \
    bankaccount.h \
    fileoperations.h \
    mainmenu.h \
    transaction.h \
    user.h
