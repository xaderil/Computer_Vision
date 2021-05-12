cmake_minimum_required(VERSION 3.16.3)

project(Computer_Vision)

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/maxon/root-6.22.08-install/cmake)
# set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
# set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
find_package(OpenCV REQUIRED)

find_package(ROOT REQUIRED)
find_package(Boost REQUIRED)
if(Boost_FOUND)
   message(STATUS ${Boost_VERSION} )
endif()

include(${ROOT_USE_FILE})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(Computer_Vision Computer_Vision.cpp Seeker.cpp Seeker.hpp Ball.cpp Ball.hpp Forecaster.hpp Forecaster.cpp Monitoring.hpp Monitoring.cpp JSON_Worker.hpp nlohmann/json.hpp)

target_link_libraries(Computer_Vision ${OpenCV_LIBS})
target_link_libraries(Computer_Vision ${Boost_LIBRARIES})
target_link_libraries(Computer_Vision ${ROOT_LIBRARIES})

