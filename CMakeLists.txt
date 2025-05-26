cmake_minimum_required(VERSION 3.22)
include(FetchContent)

set(PROJECT_NAME ccpm)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${PROJECT_NAME}
    LANGUAGES
    C
    CXX)

if(DEFINED ENV{INSTALL})
  set(INSTALL TRUE)
else()
  set(INSTALL FALSE)
endif()

if(INSTALL)
  include(GNUInstallDirs)
endif()

set(SOURCES
    src/main.cpp
    # src/PackageCompiler.cpp
    src/PackageInstaller.cpp
    src/PackageSearcher.cpp
    src/PackagesFetcher.cpp)

find_package(curlpp REQUIRED)
find_package(Boost
    REQUIRED
    COMPONENTS
    process)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    android-wordexp
    curlpp::curlpp_static
    ${Boost_LIBRARIES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(INSTALL)
  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp")
endif()