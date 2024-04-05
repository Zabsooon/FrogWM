cmake_minimum_required(VERSION 3.10)
project(frogwm VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# In case of any CMake scripts will be needed,
# implement them in ./cmake,
# like Findglog.cmake -used by `find_package(glog REQUIRED)`
# set(CMAKE_CXX_FLAGS "${CMAKE_SOURCE_DIR}/cmake")


find_package(X11 REQUIRED)
find_package(glog)

include_directories(${X11_INCLUDE_DIR})
include_directories("src")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h")

add_executable(
    ${PROJECT_NAME}

    src/main.cc
    src/window_manager.cc
    src/window_utils.cc
)

# Linking libraries...
set(LINK_LIBRARIES ${X11_LIBRARIES} glog::glog)
target_link_libraries(frogwm ${LINK_LIBRARIES})
