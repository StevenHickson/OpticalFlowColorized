cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

IF(WIN32 OR win64)
   SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll)
ELSE()
   SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
ENDIF()


find_package( OpenCV REQUIRED )
FIND_PACKAGE(CUDA REQUIRED)
 
SET(CUDA_NVCC_FLAGS "-arch=sm_20" CACHE STRING "nvcc flags" FORCE)
#SET (CUDA_VERBOSE_BUILD ON CACHE BOOL "nvcc verbose" FORCE)

project(OpticalFlow)

add_subdirectory(src)

include_directories(C:/opencv/build)
include_directories(${OpticalFlow_SOURCE_DIR}/include)

file(GLOB_RECURSE OpticalFlow_HEADERS include/*.h)
file(GLOB_RECURSE OpticalFlow_SOURCES src/*.cpp)
set (OpticalFlow_INCLUDE_DIRS "")
foreach (_headerFile ${OpticalFlow_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND OpticalFlow_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES OpticalFlow_INCLUDE_DIRS)

include_directories(${OpticalFlow_INCLUDE_DIRS})

add_executable (OpticalFlow ${OpticalFlow_SOURCES} ${OpticalFlow_HEADERS})
target_link_libraries (OpticalFlow Shlwapi ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY} ${OpenCV_LIBS})