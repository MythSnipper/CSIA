cmake_minimum_required(VERSION 3.16)

project(QuantumEngine
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS 
    Widgets 
    OpenGL 
    OpenGLWidgets
)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(QuantumEngine
    src/main.cpp
    src/MainWindow.cpp
    src/GLRenderer.cpp
    src/QT_GLRenderer.cpp
    resources.qrc
)

target_include_directories(QuantumEngine
    PRIVATE
        include
)

target_link_libraries(QuantumEngine
    PRIVATE
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
)

target_compile_options(QuantumEngine PRIVATE
    -O1
    -Wall
    -Wextra
)

target_link_options(QuantumEngine PRIVATE
)


