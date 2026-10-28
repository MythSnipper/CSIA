cmake_minimum_required(VERSION 3.16)

project(CSIA
    VERSION 1.0
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

    src/main.cpp
    src/MainWindow.cpp

    src/scene/Scene.cpp

    src/renderer/QT_GLRenderer.cpp
    src/renderer/GLRenderer.cpp

    src/renderer/Material.cpp
    src/renderer/Mesh.cpp
    src/renderer/Camera.cpp
    src/renderer/RenderObject.cpp
    src/renderer/Transform.cpp
    src/renderer/Texture2D.cpp
    src/renderer/EBO.cpp
    src/renderer/VBO.cpp
    src/renderer/VAO.cpp
    src/renderer/ShaderProgram.cpp
    src/renderer/FragmentShader.cpp
    src/renderer/VertexShader.cpp
    src/renderer/readfile.cpp
)

target_include_directories(QuantumEngine
    PRIVATE
        include
        include/renderer
        include/scene
)

target_link_libraries(QuantumEngine
    PRIVATE
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
)

target_compile_options(QuantumEngine PRIVATE
    -O0
    -Wall
    -Wextra
)

target_link_options(QuantumEngine PRIVATE
)


