#   Copyright 2020-2020 Exactpro (Exactpro Systems Limited)
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

cmake_minimum_required(VERSION 3.6)

project(th2-common-cpp)

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)

if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} doesn't have C++17 support.")
endif()

#set(BOOST_ROOT "/home/sa/usr_gcc_10_fin")

file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE SRC     "src/*.c*")

set(INSTALLATION_PATH /usr CACHE STRING "Path to install libcommon_utils")
set(LIB_SEARCH_PATH "" CACHE STRING "Paths to folders with headers and libs")

set(STARTSTATIC "-Wl,-Bstatic")
set(ENDSTATIC "-Wl,-Bdynamic")
set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")

option(EXAMPLE ON)

#set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS program_options system)
message(STATUS "Boost version: ${Boost_VERSION}")

link_directories(. ${LIB_SEARCH_PATH}/lib ${LIB_SEARCH_PATH}/lib64 ${ADDITIONAL_LIBRARY_PATH})

add_library(${PROJECT_NAME}_static STATIC ${SRC})
target_include_directories(${PROJECT_NAME}_static PRIVATE "." include ${LIB_SEARCH_PATH}/include)
target_link_libraries(${PROJECT_NAME}_static PRIVATE Boost::program_options Boost::system protobuf rabbitmq dl pthread)

set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set(INSTALLED_LIBS ${INSTALLED_LIBS} ${PROJECT_NAME}_static)

if (EXAMPLE)
    add_executable(${PROJECT_NAME}_tests ${HEADERS} main.cpp)
    target_include_directories(${PROJECT_NAME}_tests PRIVATE include ${LIB_SEARCH_PATH}/include)
    target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_static)
endif()
