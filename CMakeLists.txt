cmake_minimum_required(VERSION 3.16)

project(Projet_Fin_dAnnee VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        # Point d'entrée et fenêtre principale
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui

        # Couche données (accès au fichier eleves.txt)
        src/data/eleverepository.cpp
        src/data/eleverepository.h

        # Composants réutilisables
        src/widgets/sidebar.cpp
        src/widgets/sidebar.h

        # Pages de l'application
        src/pages/dashboardpage.cpp
        src/pages/dashboardpage.h
        src/pages/inscriptionpage.cpp
        src/pages/inscriptionpage.h
        src/pages/listeelevespage.cpp
        src/pages/listeelevespage.h

        # Ressources (feuille de style)
        resources/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Projet_Fin_dAnnee
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Projet_Fin_dAnnee APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Projet_Fin_dAnnee SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Projet_Fin_dAnnee
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Projet_Fin_dAnnee PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Permet d'écrire #include "pages/dashboardpage.h" depuis n'importe quel fichier
target_include_directories(Projet_Fin_dAnnee PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Projet_Fin_dAnnee)
endif()
set_target_properties(Projet_Fin_dAnnee PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Projet_Fin_dAnnee
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Projet_Fin_dAnnee)
endif()
