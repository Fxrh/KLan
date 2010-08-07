project( QLan )

set( QT_USE_QTNETWORK TRUE )

cmake_minimum_required(VERSION 2.6)
find_package( Qt4 REQUIRED )
add_definitions(${QT_DEFINITIONS} )
include_directories( ${CMAKE_SOURCE_DIR} ${QLan_SOURCE_DIR} ${QLan_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
include(${QT_USE_FILE})


set( QLan_SRCS main.cpp
               mainwindow.cpp
               chatwindow.cpp
               trayicon.cpp
               klansettings.cpp
               settingsdialog.cpp
               model/conmodel.cpp
               model/confilter.cpp
               model/condelegate.cpp
               model/conitem.cpp
               connection/connectionobject.cpp
               connection/conmanager.cpp 
               connection/server.cpp
               connection/client.cpp
               connection/clientconnection.cpp
               connection/serverconnection.cpp
               connection/def.h
               )
               
set( QLan_MOC_HDRS mainwindow.h
                   chatwindow.h
                   settingsdialog.h
                   model/conmodel.h
                   model/confilter.h
                   model/condelegate.h
                   connection/connectionobject.h
                   connection/conmanager.h
                   connection/server.h
                   connection/client.h
                   connection/clientconnection.h
                   connection/serverconnection.h
                   )
                   
QT4_WRAP_CPP( QLan_MOC_SRCS ${QLan_MOC_HDRS} )

add_executable( qlan ${QLan_SRCS} ${QLan_MOC_SRCS} )
target_link_libraries( qlan ${QT_LIBRARIES} )

install(TARGETS qlan DESTINATION bin)

