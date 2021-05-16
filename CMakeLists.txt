cmake_minimum_required(VERSION 3.17)
project(proj)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Build the Google tests
set(GOOGLETEST_PATH lib/googletest-master)
add_subdirectory(${GOOGLETEST_PATH})

# Build GraphViewerCpp
set(GRAPHVIEWERCPP_PATH lib/GraphViewerCpp)
add_subdirectory(${GRAPHVIEWERCPP_PATH})
include_directories(${GRAPHVIEWERCPP_PATH}/include)

file(GLOB PROJ_FILES CONFIGURE_DEPENDS
        "src/*.cpp"
        )
add_executable(PROJ
        src/main.cpp
        ${PROJ_FILES}
        src/Carrinha.h)

target_link_libraries(PROJ gtest_main gmock_main graphviewer)