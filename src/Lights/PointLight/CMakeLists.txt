add_library(pointLight SHARED
    PointLight.cpp
    ${CMAKE_SOURCE_DIR}/src/Lights/ALights.cpp
    ${CMAKE_SOURCE_DIR}/src/Vectors/Vector.cpp
    ${CMAKE_SOURCE_DIR}/src/Points/Points.cpp
    ${CMAKE_SOURCE_DIR}/src/Ray/Ray.cpp
    ${CMAKE_SOURCE_DIR}/src/BVHBuilder/BinaryTree.cpp
    ${CMAKE_SOURCE_DIR}/src/BVHBuilder/AABB.cpp
    ${CMAKE_SOURCE_DIR}/src/Builder/RayTracer.cpp
    ${CMAKE_SOURCE_DIR}/src/Parsing/ArgumentMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Color/Color.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Materials.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Texture.cpp
)
target_include_directories(pointLight PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(pointLight PROPERTIES OUTPUT_NAME "PointLight")
set_target_properties(pointLight PROPERTIES PREFIX "Lights/lib")