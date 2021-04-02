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
#find_package(Armadillo REQUIRED)
#find_package(Gnuplot REQUIRED)
# find_package(sciplot REQUIRED)
# find_package(Python COMPONENTS Interpreter Development NumPy)
find_package(ROOT REQUIRED)
# find_package(Boost CONFIG REQUIRED regex system filesystem)
#if (Python_FOUND)
    #message("${Python_VERSION}")
#endif()

include(${ROOT_USE_FILE})

include_directories(${OpenCV_INCLUDE_DIRS})
#include_directories(${Gnuplot_INCLUDE_DIRS})
# include_directories(${ARMADILLO_INCLUDE_DIRS})
#include_directories(${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
add_executable(Computer_Vision Computer_Vision.cpp Seeker.cpp Seeker.hpp Ball.cpp Ball.hpp Forecaster.cpp Forecaster.hpp)

target_link_libraries(Computer_Vision ${OpenCV_LIBS})
#target_link_libraries(Computer_Vision ${ARMADILLO_LIBRARIES})
#target_link_libraries(Computer_Vision ${Gnuplot_LIBS})
#target_link_libraries(Computer_Vision sciplot::sciplot)
#target_link_libraries(Computer_Vision ${PYTHON_LIBRARIES})
target_link_libraries(Computer_Vision ${ROOT_LIBRARIES})

