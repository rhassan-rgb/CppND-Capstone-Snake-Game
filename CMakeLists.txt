cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall" "-pthread")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_library(SDL2_TTF_LIBRARY
    NAMES SDL2_ttf
    PATHS /usr/lib /usr/local/lib  
    REQUIRED
)

# Check if SDL2_ttf library is found
if(SDL2_TTF_LIBRARY)
    message(STATUS "SDL2_ttf library found: ${SDL2_TTF_LIBRARY}")
else()
    message(FATAL_ERROR "SDL2_ttf library not found")
endif()


include_directories(${SDL2_INCLUDE_DIRS} src)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(SnakeGame ${SOURCE_FILES})
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(SnakeGame ${SDL2_LIBRARIES}  ${SDL2_TTF_LIBRARY})
