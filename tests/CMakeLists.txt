cmake_minimum_required(VERSION 3.16)

# Configuration pour les tests Criterion
include_directories(
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/src/Vectors/
    ${CMAKE_SOURCE_DIR}/src/Points/
    ${CMAKE_SOURCE_DIR}/src/Interfaces/
    ${CMAKE_SOURCE_DIR}/src/Parsing/
    ${CMAKE_SOURCE_DIR}/src/Color/
    ${CMAKE_SOURCE_DIR}/src/Ray/
    ${CMAKE_SOURCE_DIR}/src/BVHBuilder/
    ${CMAKE_SOURCE_DIR}/src/Rectangle3D/
    ${CMAKE_SOURCE_DIR}/src/Camera/
)

set(TEST_FILES
    test_vector.cpp
    test_point.cpp
    test_color.cpp
    more_test.cpp
)

set(TEST_SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/Vectors/Vector.cpp
    ${CMAKE_SOURCE_DIR}/src/Points/Points.cpp
    ${CMAKE_SOURCE_DIR}/src/Color/Color.cpp
    ${CMAKE_SOURCE_DIR}/src/BVHBuilder/AABB.cpp
    ${CMAKE_SOURCE_DIR}/src/Rectangle3D/Screen.cpp
    ${CMAKE_SOURCE_DIR}/src/Parsing/ArgumentMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Camera/Camera.cpp
    ${CMAKE_SOURCE_DIR}/src/Ray/Ray.cpp
    ${CMAKE_SOURCE_DIR}/src/BVHBuilder/BinaryTree.cpp
    ${CMAKE_SOURCE_DIR}/src/Materials/Texture.cpp
)

# Création de l'exécutable de test
add_executable(unittest ${TEST_FILES} ${TEST_SRC_FILES})

# Ajout des flags de coverage spécifiquement pour les tests
target_compile_options(unittest PRIVATE --coverage)
target_link_options(unittest PRIVATE --coverage)

# target_link_libraries(raytracer
    
# )

# Lien avec les bibliothèques nécessaires
target_link_libraries(unittest criterion pthread gcov sfml-graphics)

# Ajout d'un test CMake
add_test(NAME unittest COMMAND unittest)

