QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
INCLUDEPATH += $${_PRO_FILE_PWD_}/src
win32: RC_ICONS = $$PWD/bin/images/app.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/dialog/catalogdialog.cpp \
    src/dialog/folderdialog.cpp \
    src/dialog/linkdialog.cpp \
    src/application.cpp \
    src/data/data.cpp \
    src/data/storage.cpp \
    src/data/catalog.cpp \
    src/data/folder.cpp \
    src/data/link.cpp \
    src/dialog/mover.cpp \
    src/dialog/namedialog.cpp \
    src/helpers/counter.cpp \
    src/view/catalogsview.cpp \
    src/view/folderview.cpp \
    src/view/linksview.cpp \
    src/main.cpp \
    src/view/mainwindow.cpp \
    src/model/model.cpp     \
    src/helpers/posaction.cpp

HEADERS += \
    src/dialog/catalogdialog.h \
    src/dialog/folderdialog.h \
    src/dialog/linkdialog.h \
    src/application.h \
    src/data/data.h \
    src/data/storage.h \
    src/data/catalog.h \
    src/data/folder.h \
    src/data/link.h \
    src/dialog/mover.h \
    src/dialog/namedialog.h \
    src/helpers/counter.h \
    src/view/catalogsview.h \
    src/view/folderview.h \
    src/view/linksview.h \
    src/view/mainwindow.h \
    src/model/model.h  \
    src/helpers/posaction.h

FORMS += \
    src/dialog/catalogdialog.ui \
    src/dialog/folderdialog.ui \
    src/dialog/linkdialog.ui \
    src/dialog/mover.ui \
    src/dialog/namedialog.ui \
    src/view/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    bin/img.qrc
