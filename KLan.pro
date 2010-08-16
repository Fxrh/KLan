QT += network phonon

TEMPLATE = app
TARGET = qlan
DEPENDPATH += . \
              connection \
              model 

INCLUDEPATH += . connection model

# Input
HEADERS += chatwindow.h \
           klansettings.h \
           mainwindow.h \
           settingsdialog.h \
           sound.h \
           trayicon.h \
           connection/client.h \
           connection/clientconnection.h \
           connection/conmanager.h \
           connection/connectionobject.h \
           connection/def.h \
           connection/server.h \
           connection/serverconnection.h \
           model/condelegate.h \
           model/confilter.h \
           model/conitem.h \
           model/conmodel.h 

SOURCES += chatwindow.cpp \
           klansettings.cpp \
           main.cpp \
           mainwindow.cpp \
           settingsdialog.cpp \
           sound.cpp \
           trayicon.cpp \
           connection/client.cpp \
           connection/clientconnection.cpp \
           connection/conmanager.cpp \
           connection/connectionobject.cpp \
           connection/server.cpp \
           connection/serverconnection.cpp \
           model/condelegate.cpp \
           model/confilter.cpp \
           model/conitem.cpp \
           model/conmodel.cpp \

RESOURCES += klan.qrc
