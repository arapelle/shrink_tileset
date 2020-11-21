cmake_minimum_required(VERSION 3.19)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

include(CMakePrintHelpers)
include(cmtk/Project)

#-----
# PROJECT

set_build_type_if_undefined()

#-----
# C++ PROJECT

project(shrink_tileset
#        VERSION "0.1.0"
#        DESCRIPTION ""
#        HOMEPAGE_URL ""
        LANGUAGES CXX)

message(STATUS "BUILD   : ${CMAKE_BUILD_TYPE}")
message(STATUS "CPPCOMP : ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "PROJECT : ${PROJECT_NAME} ${PROJECT_VERSION}")

#-----
add_executable(${PROJECT_NAME} main.cpp main.hpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

find_package(ImageMagick COMPONENTS Magick++)
target_compile_definitions(${PROJECT_NAME} PUBLIC MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE_HDRI_ENABLE=0)
target_include_directories(${PROJECT_NAME} PUBLIC ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${ImageMagick_LIBRARIES})

file(CREATE_LINK "${CMAKE_CURRENT_LIST_DIR}/emona.png" "${CMAKE_CURRENT_BINARY_DIR}/emona.png" COPY_ON_ERROR SYMBOLIC)

#-----
