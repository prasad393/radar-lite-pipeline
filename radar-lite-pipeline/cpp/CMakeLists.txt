cmake_minimum_required(VERSION 3.16)
project(radar_lite CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(radar_core
    chirp_gen.cpp
    fft_wrapper.cpp
    cfar_ca.cpp
    tracker_kf.cpp
)
target_include_directories(radar_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(radar-lite main.cpp)
target_link_libraries(radar-lite PRIVATE radar_core)
