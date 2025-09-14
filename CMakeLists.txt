cmake_minimum_required(VERSION 3.16)

# Set a default version if VERSION_SHORT is not defined
if(NOT DEFINED VERSION_SHORT)
    set(VERSION_SHORT 0.1.0)
endif()

project(s_core VERSION ${VERSION_SHORT})

# Find Qt version (prefer Qt6, fallback to Qt5)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
if(QT_FOUND)
    if(Qt6_FOUND)
        set(QT_VERSION_MAJOR 6)
    elseif(Qt5_FOUND)
        set(QT_VERSION_MAJOR 5)
    endif()
else()
    message(FATAL_ERROR "Qt5 or Qt6 not found!")
endif()

find_package(Qt${QT_VERSION_MAJOR} 5.5 COMPONENTS Core Gui Widgets REQUIRED)

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

)

target_include_directories(SCore PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/external/ads"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ui"
)

# Check if qtadvanceddocking-qt target exists, otherwise add_subdirectory
if(NOT TARGET qtadvanceddocking-qt${QT_VERSION_MAJOR})
    add_subdirectory(external/ads)
endif()
target_link_libraries(SCore PRIVATE qtadvanceddocking-qt${QT_VERSION_MAJOR})

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
