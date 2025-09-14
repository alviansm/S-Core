cmake_minimum_required(VERSION 3.16)

# Set a default version if VERSION_SHORT is not defined
if(NOT DEFINED VERSION_SHORT)
    set(VERSION_SHORT 0.1.0)
endif()

project(s_core VERSION ${VERSION_SHORT})

# Find Qt version (prefer Qt6, fallback to Qt5)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core WebEngineWidgets REQUIRED)
if(QT_FOUND)
    if(Qt6_FOUND)
        set(QT_VERSION_MAJOR 6)
    elseif(Qt5_FOUND)
        set(QT_VERSION_MAJOR 5)
    endif()
else()
    message(FATAL_ERROR "Qt5 or Qt6 not found!")
endif()

find_package(Qt${QT_VERSION_MAJOR} 5.5 COMPONENTS Core Gui Widgets WebEngineWidgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Source files
set(SOURCES
    src/main.cpp
    src/ui/MainWindow.cpp


)

# Header files  
set(HEADERS
    src/ui/MainWindow.h


)

# UI files
set(UI_FILES
    src/ui/MainWindow.ui
)

add_executable(SCore WIN32 
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    res/resources.qrc
    src/ui/WelcomePage.h src/ui/WelcomePage.cpp src/ui/WelcomePage.ui
    src/ui/MapboxWidget.h src/ui/MapboxWidget.cpp

)

target_include_directories(SCore PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/external/ads"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ui"
)

# Check if qtadvanceddocking-qt target exists, otherwise add_subdirectory
if(NOT TARGET qtadvanceddocking-qt${QT_VERSION_MAJOR})
    add_subdirectory(external/ads)
endif()
target_link_libraries(SCore PRIVATE qtadvanceddocking-qt${QT_VERSION_MAJOR} Qt${QT_VERSION_MAJOR}::WebEngineWidgets)

target_link_libraries(SCore PUBLIC Qt${QT_VERSION_MAJOR}::Core 
                                   Qt${QT_VERSION_MAJOR}::Gui 
                                   Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(SCore PROPERTIES 
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION ${VERSION_SHORT}
    EXPORT_NAME "Ship Energy Optimization Platform"
)

# Set output directories
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/output")
set_target_properties(SCore PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/bin"
)

if(WIN32)
    # Set icon path - pastikan file icon ada di lokasi ini
    set(WINDOWS_ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/res/icons/s-core_app_icon.ico")
    set(WINDOWS_RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

    # Buat resource file jika belum ada
    if(NOT EXISTS ${WINDOWS_RESOURCE_FILE})
        file(WRITE ${WINDOWS_RESOURCE_FILE}
            "IDI_ICON1 ICON DISCARDABLE \"res/icons/s-core_app_icon.ico\"\n"
        )
    endif()

    # Tambahkan resource file ke executable
    target_sources(SCore PRIVATE ${WINDOWS_RESOURCE_FILE})

    # Set icon untuk executable
    set_target_properties(SCore PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
