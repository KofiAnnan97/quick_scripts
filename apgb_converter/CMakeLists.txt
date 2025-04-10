cmake_minimum_required(VERSION 3.16)

project(apgb_converter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    include/mainwindow.h
    ui/mainwindow.ui
)

set(DOC_SOURCES
    README.md
    img/editor.png
    img/convert.png
)

set(DYNAMIC_IMG_SOURCES
  img/test.txt
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(apgb_converter
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${DOC_SOURCES}
        ${DYNAMIC_IMG_SOURCES}
        include/palettes.h
        include/fileimporter.h
        fileimporter.cpp
        include/save.h
        save.cpp
        include/imageimporter.h 
        imageimporter.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET apgb_converter APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(apgb_converter SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(apgb_converter
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(apgb_converter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(apgb_converter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apgb_converter)
endif()
set_target_properties(apgb_converter PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS apgb_converter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(apgb_converter)
endif()
