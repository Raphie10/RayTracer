add_library(plane SHARED
    Plane.cpp
    ${CMAKE_SOURCE_DIR}/src/Vectors/Vector.cpp
    ${CMAKE_SOURCE_DIR}/src/Points/Points.cpp
    ${CMAKE_SOURCE_DIR}/src/Parsing/ArgumentMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Color/Color.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Materials.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Texture.cpp
)
target_include_directories(plane PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(plane PROPERTIES OUTPUT_NAME "Plane")
set_target_properties(plane PROPERTIES PREFIX "Primitives/lib")