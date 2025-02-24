cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

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


add_executable(mathness ${SRC_FILES} "src/Parser/Utils/Utils.h")

find_package(fmt CONFIG REQUIRED)
target_link_libraries(mathness PRIVATE fmt::fmt)

target_include_directories(mathness PUBLIC src)