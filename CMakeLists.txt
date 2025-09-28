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

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts Pdf PdfWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(SCore WIN32
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    res/resources.qrc
    src/ui/WelcomePage.h src/ui/WelcomePage.cpp src/ui/WelcomePage.ui
    src/ui/MapboxWidget.h src/ui/MapboxWidget.cpp
    src/ui/Pages/TechnicalPage.h src/ui/Pages/TechnicalPage.cpp src/ui/Pages/TechnicalPage.ui
    src/ui/Pages/DecisionSupportPage.h src/ui/Pages/DecisionSupportPage.cpp src/ui/Pages/DecisionSupportPage.ui
    src/ui/Pages/VoyagePlanningPage.h src/ui/Pages/VoyagePlanningPage.cpp src/ui/Pages/VoyagePlanningPage.ui
    src/ui/Pages/HistoryPage.h src/ui/Pages/HistoryPage.cpp src/ui/Pages/HistoryPage.ui
    src/ui/Pages/SettingPage.h src/ui/Pages/SettingPage.cpp src/ui/Pages/SettingPage.ui
    src/ui/Pages/DashboardPage.h src/ui/Pages/DashboardPage.cpp src/ui/Pages/DashboardPage.ui
    src/ui/CircleProgressBar.h src/ui/CircleProgressBar.cpp
    src/ui/MapboxWidgetSimple.h src/ui/MapboxWidgetSimple.cpp
    src/ui/Pages/Components/VoyageItemWidget.h src/ui/Pages/Components/VoyageItemWidget.cpp
    src/ui/Pages/Components/DialogAddPortRotation.h src/ui/Pages/Components/DialogAddPortRotation.cpp src/ui/Pages/Components/DialogAddPortRotation.ui
    src/ui/Pages/Components/DialogAddCargo.h src/ui/Pages/Components/DialogAddCargo.cpp src/ui/Pages/Components/DialogAddCargo.ui
    src/ui/Pages/Components/DialogAddFuelPlan.h src/ui/Pages/Components/DialogAddFuelPlan.cpp src/ui/Pages/Components/DialogAddFuelPlan.ui
    src/ui/Pages/Components/DialogAddNewVoyage.h src/ui/Pages/Components/DialogAddNewVoyage.cpp src/ui/Pages/Components/DialogAddNewVoyage.ui
    src/ui/SpeedometerWidget.h src/ui/SpeedometerWidget.cpp
    src/ui/EngineStatusWidget.h src/ui/EngineStatusWidget.cpp
    src/ui/PropulsionPIDWidget.h src/ui/PropulsionPIDWidget.cpp
    src/service/MockApiService.h src/service/MockApiService.cpp
    src/ui/Pages/Components/DialogBrowsePort.h src/ui/Pages/Components/DialogBrowsePort.cpp src/ui/Pages/Components/DialogBrowsePort.ui
    src/service/PortSearch.h src/service/PortSearch.cpp
    src/ui/KPIOverviewFrame.h src/ui/KPIOverviewFrame.cpp src/ui/KPIOverviewFrame.ui
    src/ui/WeatherFrame.h src/ui/WeatherFrame.cpp src/ui/WeatherFrame.ui
    src/ui/AlertAndRecomendationFrame.h src/ui/AlertAndRecomendationFrame.cpp src/ui/AlertAndRecomendationFrame.ui
    src/ui/HintboxDialog.h src/ui/HintboxDialog.cpp src/ui/HintboxDialog.ui
    src/ui/RecommendationCard.h src/ui/RecommendationCard.cpp
    src/ui/CarbonIntensityGauge.h src/ui/CarbonIntensityGauge.cpp


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
target_link_libraries(SCore PRIVATE Qt6::Core Qt6::Widgets Qt6::Charts Qt6::Pdf Qt6::PdfWidgets)

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
