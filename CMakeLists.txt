project( KLan )

cmake_minimum_required(VERSION 2.6)
find_package( KDE4 REQUIRED )
include( KDE4Defaults )
include_directories( ${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} )


set( KLan_SRCS main.cpp
               mainwindow.cpp
               chatwindow.cpp
               trayicon.cpp
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
               sendingfiles/sendclient.cpp
               sendingfiles/sendserver.cpp
               sendingfiles/sendfiles.cpp
               sendingfiles/senditem.cpp
               widgets/sizedeplabel.cpp
               )
         
set( KLan_SOUND data/sounds/klan_connect.ogg
                data/sounds/klan_chat.ogg
                )

kde4_add_kcfg_files(KLan_SRCS klansettings.kcfgc)

kde4_add_executable( klan ${KLan_SRCS} )
target_link_libraries( klan ${KDE4_KDEUI_LIBS} )

install(TARGETS klan DESTINATION ${BIN_INSTALL_DIR})
install(FILES klanui.rc DESTINATION  ${DATA_INSTALL_DIR}/klan)
install( FILES klan.notifyrc DESTINATION ${DATA_INSTALL_DIR}/klan )
install(FILES klan.kcfg DESTINATION ${KCFG_INSTALL_DIR})
install( FILES ${KLan_SOUND} DESTINATION ${SOUND_INSTALL_DIR} )

