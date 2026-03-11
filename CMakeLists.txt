cmake_minimum_required(VERSION 3.16)

project(POO-TP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Multimedia)

set(TS_FILES POO-TP_es_AR.ts)

set(PROJECT_SOURCES
    main.cpp
    Mainwindow.cpp
    Mainwindow.h
    Mainwindow.ui
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(POO-TP
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        PantallaMenu/PantallaMenu.h PantallaMenu/PantallaMenu.cpp PantallaMenu/PantallaMenu.ui
        PantallaAjusteDePartida/PantallaAjusteDePartida.h PantallaAjusteDePartida/PantallaAjusteDePartida.cpp PantallaAjusteDePartida/PantallaAjusteDePartida.ui
        PantallaPartida/PantallaPartida.h PantallaPartida/PantallaPartida.cpp PantallaPartida/PantallaPartida.ui
        logica/Jugador/jugador.h logica/Jugador/jugador.cpp
        logica/Tablero/tablero.h logica/Tablero/tablero.cpp
        logica/Casillas/casilla.h logica/Casillas/casilla.cpp
        logica/Casillas/casillanormal.h logica/Casillas/casillanormal.cpp
        logica/Casillas/casilladeoca.h logica/Casillas/casilladeoca.cpp
        logica/Casillas/casillapuente.h logica/Casillas/casillapuente.cpp
        logica/Casillas/casillaposada.h logica/Casillas/casillaposada.cpp
        logica/Casillas/casillapozo.h logica/Casillas/casillapozo.cpp
        logica/Casillas/casillalaberinto.h logica/Casillas/casillalaberinto.cpp
        CasillaUI/casillaui.h CasillaUI/casillaui.cpp CasillaUI/casillaui.ui
        TableroUI/tableroui.h TableroUI/tableroui.cpp TableroUI/tableroui.ui
        PantallaTransicion/pantallatransicion.h PantallaTransicion/pantallatransicion.cpp PantallaTransicion/pantallatransicion.ui
        PantallaPausa/pantallapausa.h PantallaPausa/pantallapausa.cpp PantallaPausa/pantallapausa.ui
        logica/JuegoOca/juegooca.h logica/JuegoOca/juegooca.cpp
        Recursos.qrc
        ManejadorDeSonidos/manejadordesonidos.h ManejadorDeSonidos/manejadordesonidos.cpp
        logica/Casillas/casillacarcel.h logica/Casillas/casillacarcel.cpp
        logica/Casillas/casillacalavera.h logica/Casillas/casillacalavera.cpp
        logica/Casillas/casillajardindeoca.h logica/Casillas/casillajardindeoca.cpp
        logica/Dado/dado.h logica/Dado/dado.cpp
        JugadorUI/jugadorui.h JugadorUI/jugadorui.cpp JugadorUI/jugadorui.ui
        PantallaFinal/pantallafinal.h PantallaFinal/pantallafinal.cpp PantallaFinal/pantallafinal.ui
        PantallaPartida/notificacion.h PantallaPartida/notificacion.cpp PantallaPartida/notificacion.ui
        GestorDeGuardado/GestorDeGuardado.h
        GestorDeGuardado/GestorDeGuardado.cpp
        PantallaAjusteDePartida/ConfiguracionJuego.h

    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(POO-TP SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(POO-TP
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(POO-TP PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.POO-TP)
endif()

set_target_properties(POO-TP PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS POO-TP
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(POO-TP)
endif()
