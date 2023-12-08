cmake_minimum_required(VERSION 3.16)
project(cutie-messaging-daemon VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QT_DEFAULT_MAJOR_VERSION 6)

find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Quick)
pkg_check_modules(CUTIEMODEM cutiemodem REQUIRED IMPORTED_TARGET)
pkg_check_modules(NOTIFICATION cutienotification REQUIRED IMPORTED_TARGET)
pkg_check_modules(CUTIESTORE cutiestore REQUIRED IMPORTED_TARGET)

qt_standard_project_setup()

set(cutie-messaging-daemon_SOURCES 
        src/main.cpp
        src/messaging.cpp
        src/messaging.h
)

qt_add_executable(cutie-messaging-daemon ${cutie-messaging-daemon_SOURCES})

target_include_directories(cutie-messaging-daemon PUBLIC src)

target_link_libraries(cutie-messaging-daemon PUBLIC 
        Qt::Core 
        Qt::Quick
        PkgConfig::CUTIEMODEM
        PkgConfig::NOTIFICATION
        PkgConfig::CUTIESTORE
)

install(TARGETS cutie-messaging-daemon RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
