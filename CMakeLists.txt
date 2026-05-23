cmake_minimum_required(VERSION 3.16)
project(TiwutTerminal VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(TiwutTerminal
    main.cpp
    tiwut_engine.cpp
    tiwut_engine.h
    terminal_window.cpp
    terminal_window.h
)

target_link_libraries(TiwutTerminal PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)