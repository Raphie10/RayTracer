add_library(directionalLight SHARED
    DirectionalLight.cpp
    ${CMAKE_SOURCE_DIR}/src/Lights/ALights.cpp
    ${CMAKE_SOURCE_DIR}/src/Vectors/Vector.cpp
    ${CMAKE_SOURCE_DIR}/src/Points/Points.cpp
    ${CMAKE_SOURCE_DIR}/src/Ray/Ray.cpp
    ${CMAKE_SOURCE_DIR}/src/Builder/RayTracer.cpp
    ${CMAKE_SOURCE_DIR}/src/Parsing/ArgumentMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Color/Color.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Materials.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Texture.cpp
)
target_include_directories(directionalLight PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(directionalLight PROPERTIES OUTPUT_NAME "DirectionalLight")
set_target_properties(directionalLight PROPERTIES PREFIX "Lights/lib")