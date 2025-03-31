cmake_minimum_required(VERSION 3.16)

project(RobotControlStation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
# 查找 Qt 包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
    MultimediaWidgets
    Network
    SerialPort
)


set(PROJECT_SOURCES
        main.cpp
        robot_ground_station.cpp
        robot_ground_station.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RobotControlStation
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        manager_rtsp/widget_rtsp.hpp manager_rtsp/widget_rtsp.cpp
        manager_rtsp/widget_overlay.hpp manager_rtsp/widget_overlay.cpp
        manager_gimbal/gimbal_control_siyi.hpp
        manager_gimbal/interface_gimabal_siyi.hpp manager_gimbal/interface_gimabal_siyi.cpp
        manager_gimbal/widget_gimbal.hpp manager_gimbal/widget_gimbal.cpp
        manager_deep/widget_deeprobot.hpp manager_deep/widget_deeprobot.cpp
        manager_link/widget_link.hpp manager_link/widget_link.cpp
        manager_link/remote_control_siyi.hpp
        manager_link/interface_remote_control_siyi.hpp manager_link/interface_remote_control_siyi.cpp
        manager_control_mode.hpp manager_control_mode.cpp
        manager_deep/interface_robot_deep.hpp manager_deep/interface_robot_deep.cpp
        res/res.qrc
    )
# Define target properties for Android with Qt 6 启用了Android包源目录配置
set_property(TARGET RobotControlStation APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
             ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(RobotControlStation SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(RobotControlStation
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# target_link_libraries(RobotControlStation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# 链接 Qt 库

target_link_libraries(RobotControlStation PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::SerialPort
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.RobotControlStation)
endif()
set_target_properties(RobotControlStation PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS RobotControlStation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RobotControlStation)
endif()