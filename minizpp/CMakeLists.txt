cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME minizpp)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/Extractor.cpp)

find_package(miniz REQUIRED)

add_library(${PROJECT_NAME}
    ${SOURCES})

add_library(miniz::${PROJECT_NAME}
    ALIAS
    ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    miniz::miniz)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)