cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSION OFF)
#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

project(advent_of_code_2019)

file(GLOB SOURCES *.cpp)

foreach(source ${SOURCES})
    get_filename_component(filename ${source} NAME)
    get_filename_component(targetname ${source} NAME_WE)

    add_executable(${targetname} ${filename})

    target_include_directories(${targetname}
        PUBLIC
            ..
    )
    
endforeach()
