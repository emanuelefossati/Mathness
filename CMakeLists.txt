cmake_minimum_required(VERSION 3.10)

project(Mathness VERSION 0.1.0 LANGUAGES CXX)

file(GLOB_RECURSE SRC_FILES src/**.cpp src/**.h)

set(GLOBAL PROPERTY USE_FOLDERS ON)
foreach(file ${SRC_FILES})
    get_filename_component(file_directory ${file} DIRECTORY)
    file(RELATIVE_PATH relative_path ${CMAKE_CURRENT_SOURCE_DIR}/src ${file_directory})

    source_group(${relative_path} FILES ${file})
endforeach() 

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.1
)

FetchContent_MakeAvailable(fmt)

add_executable(mathness ${SRC_FILES})
target_link_libraries(mathness PRIVATE fmt::fmt)

target_include_directories(mathness PUBLIC src)