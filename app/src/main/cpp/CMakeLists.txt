cmake_minimum_required(VERSION 3.10.2)

project("testga")

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -L${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -L${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}")

file(GLOB srcFiles *.cpp)

add_library( # Sets the name of the library.
        testga
        SHARED
        ${srcFiles})

add_library(event2 SHARED IMPORTED)
set_target_properties(event2 PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/libevent2.so)

find_library( # Sets the name of the path variable.
        log-lib
        log)

target_link_libraries( # Specifies the target library.
        testga
        -Wl,--start-group
        ssl crypto agora_facility_static
        -Wl,--end-group
        event2
        z
        ${log-lib})