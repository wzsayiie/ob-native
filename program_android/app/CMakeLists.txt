cmake_minimum_required(VERSION 3.4.1)

### configuration begin.
set(lib_name main)

list(APPEND inc_path ../../HEADERS)

list(APPEND src_aims ../../appsrc/cmain/cmain/*.cpp)
list(APPEND src_aims ../../sharedsrc/basis/basis/*.c)
list(APPEND src_aims ../../sharedsrc/cmeta/cmeta/*.cpp)
list(APPEND src_aims ../../sharedsrc/cplus/cplus/*.cpp)
list(APPEND src_aims ../../sharedsrc/net/net/*.c)
list(APPEND src_aims ../../sharedsrc/osbind/osbind/*.c)
list(APPEND src_aims ../../sharedsrc/osbind/osbind_android/*.c)
list(APPEND src_aims ../../sharedsrc/osplat/osplat/*.c)
list(APPEND src_aims ../../sharedsrc/osplat/osplat_android/*.c)
list(APPEND src_aims ../../sharedsrc/uidev/uidev/*.c)
list(APPEND src_aims ../../sharedsrc/uidev/uidev_android/*.c)

list(APPEND sys_libs log)
### configutation end.

add_compile_options(-Wno-nonportable-include-path) # ignore when header path contains "\".
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${inc_path})

file(GLOB_RECURSE src_files ${src_aims})
add_library(${lib_name} SHARED ${src_files})

target_link_libraries(${lib_name} ${sys_libs})
