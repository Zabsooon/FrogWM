cmake_minimum_required(VERSION 3.10)
project(FrogWM VERSION 1.0)

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

find_package(glog 0.7.0 REQUIRED)

set(SOURCE_FILES
    main.cpp
    window_manager.cpp
)

set(HEADER_FILES
    window_manager.hpp
)

add_executable(FrogWM
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_link_libraries(FrogWM ${X11_LIBRARIES})
target_link_libraries(FrogWM glog::glog)
