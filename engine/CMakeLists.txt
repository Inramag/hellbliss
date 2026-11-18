add_library(hellbliss STATIC
    src/stb/stb_image.cpp

    src/engine.cpp
    src/core/window.cpp
    src/scene.cpp

    src/object.cpp

    src/resources.cpp
    src/resources/texture.cpp
    src/resources/shader.cpp

    src/render/renderer.cpp

    src/scripts/sprite.cpp
)

target_include_directories(hellbliss PUBLIC
    include
)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(hellbliss PUBLIC
    glfw
    OpenGL::GL
)

add_library(glad STATIC
    third_party/glad/src/gl.c
)

target_include_directories(glad PUBLIC
    third_party/glad/include
)

target_link_libraries(hellbliss PUBLIC glad)
