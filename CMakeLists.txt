cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(BUILD_SHARED_LIBS OFF)

project(labwork_10_IgorM27)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

include_directories(inc)

add_executable(labwork_10_IgorM27
        inc/Weather.cpp
        inc/Weather.h
        main.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/inc/Config.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba)
FetchContent_MakeAvailable(cpr)

target_link_libraries(labwork_10_IgorM27 PRIVATE cpr::cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(labwork_10_IgorM27 PRIVATE nlohmann_json::nlohmann_json)
