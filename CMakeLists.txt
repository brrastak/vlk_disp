cmake_minimum_required(VERSION 3.5)

project(vlk_disp)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        display.cpp
        display.h
        drawthread.cpp
        drawthread.h
        plot_buffer.c
        plot_buffer.h
        plot_area.c
        plot_area.h
        graph_common.c
        graph_common.h
        plot.c
        plot.h
        mock.cpp
        mock.h
        mock_cpp.h
        grid.c
        grid.h
        hx711.c
        hx711.h
        main.h
        fake_critical.c
        critical.h
        interface.c
        interface.h
        config.h
)
list(TRANSFORM PROJECT_SOURCES PREPEND "src/")

add_executable(vlk_disp
            ${PROJECT_SOURCES}
        )

target_link_libraries(vlk_disp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
