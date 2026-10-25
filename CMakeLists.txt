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
    resources.qrc
    src/QT_GLRenderer.cpp
    src/GLRenderer.cpp
    src/Material.cpp
    src/Mesh.cpp
    src/Camera.cpp
    src/RenderObject.cpp
    src/Transform.cpp
    src/Texture2D.cpp
    src/EBO.cpp
    src/VBO.cpp
    src/VAO.cpp
    src/ShaderProgram.cpp
    src/FragmentShader.cpp
    src/VertexShader.cpp
    src/readfile.cpp
    src/main.cpp
    src/MainWindow.cpp
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


