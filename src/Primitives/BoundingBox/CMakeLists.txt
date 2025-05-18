add_library(boundingbox SHARED
    BoundingBox.cpp
    ${CMAKE_SOURCE_DIR}/src/BVHBuilder/AABB.cpp
    ${CMAKE_SOURCE_DIR}/src/Vectors/Vector.cpp
    ${CMAKE_SOURCE_DIR}/src/Points/Points.cpp
    ${CMAKE_SOURCE_DIR}/src/Parsing/ArgumentMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Color/Color.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Materials.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Texture.cpp
)
target_link_libraries(raytracer
    sfml-graphics
)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

target_include_directories(boundingbox PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(boundingbox PROPERTIES OUTPUT_NAME "BoundingBox")
set_target_properties(boundingbox PROPERTIES PREFIX "Primitives/lib")