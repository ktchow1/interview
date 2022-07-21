cmake_minimum_required(VERSION 3.18.2)
project(interview)


set(CMAKE_C_COMPILER /usr/bin/gcc-10)
set(CMAKE_CXX_COMPILER /usr/bin/g++-10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions(-std=c++20)
add_definitions(-g)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_C_FLAGS   "-g -O0")
    set(CMAKE_CXX_FLAGS "-g -O0")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(CMAKE_C_FLAGS   "-g -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS "-g -O3 -DNDEBUG")
endif()


###################
### (1) include ###
###################
include_directories(
    include 
    /mnt/d/dev/boost_1_72_0 
)

##################
### (2) source ###
##################
file(GLOB SOURCES
     "src/*.cpp"
     "src_00_algorithm/*.cpp"
     "src_01_dynprog_vec/*.cpp"
#    "src_02_dynprog/*.cpp"
     "src_11_fundalmental/*.cpp"
     "src_12_rvalue/*.cpp"
     "src_13_type_deduction/*.cpp"
     "src_14_fundalmental/*.cpp"
     "src_interview/*.cpp"
)
add_executable(Test ${SOURCES})       # for building executable
# add_library(Test STATIC ${SOURCES}) # for building static lib .a
# add_library(Test SHARED ${SOURCES}) # for building shared lib .so

##################################
### (3) link path and link lib ###
##################################
target_link_libraries(Test -L/lib/x86_64-linux-gnu)
target_link_libraries(Test -lpthread)





