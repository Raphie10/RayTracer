add_library(sphere SHARED
    Sphere.cpp
    ${CMAKE_SOURCE_DIR}/src/Vectors/Vector.cpp
    ${CMAKE_SOURCE_DIR}/src/Points/Points.cpp
    ${CMAKE_SOURCE_DIR}/src/Parsing/ArgumentMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Color/Color.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Materials.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Texture.cpp
)

add_compile_options(-Wall -Wextra  -Wpedantic -O3)

target_include_directories(sphere PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(sphere PROPERTIES OUTPUT_NAME "Sphere")
set_target_properties(sphere PROPERTIES PREFIX "Primitives/lib")
