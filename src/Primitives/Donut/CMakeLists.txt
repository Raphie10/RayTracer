add_library(donut SHARED
    Donut.cpp
    ${CMAKE_SOURCE_DIR}/src/Vectors/Vector.cpp
    ${CMAKE_SOURCE_DIR}/src/Points/Points.cpp
    ${CMAKE_SOURCE_DIR}/src/Parsing/ArgumentMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Color/Color.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Materials.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Texture.cpp
)
target_include_directories(donut PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(donut PROPERTIES OUTPUT_NAME "Donut")
set_target_properties(donut PROPERTIES PREFIX "Primitives/lib")