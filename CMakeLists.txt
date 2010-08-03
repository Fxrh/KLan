project( KLan )

cmake_minimum_required(VERSION 2.6)
find_package( KDE4 REQUIRED )
include( KDE4Defaults )
include_directories( ${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} )


set( KLan_SRCS main.cpp
               mainwindow.cpp
               chatwindow.cpp
               trayicon.cpp
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

kde4_add_executable( klan ${KLan_SRCS} )
target_link_libraries( klan ${KDE4_KDEUI_LIBS} )

