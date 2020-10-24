cmake_minimum_required(VERSION 3.4.1)

### configuration begin.
set(lib_name main)

list(APPEND inc_path ../../GHEADERS)

list(APPEND src_aims ../../mainsrc/basis/basis/*.c)
list(APPEND src_aims ../../mainsrc/main/main/*.cpp)
list(APPEND src_aims ../../mainsrc/uidev/uidev/*.c)
list(APPEND src_aims ../../mainsrc/uidev/uidev_android/*.c)

list(APPEND sys_libs log)
### configutation end.

# on windows, the path of header maybe includes "\".
# by default, the compiler will consisder it nonportable and show warning.
add_compile_options(-Wno-nonportable-include-path)

include_directories(${inc_path})

file(GLOB_RECURSE src_files ${src_aims})
add_library(${lib_name} SHARED ${src_files})

target_link_libraries(${lib_name} ${sys_libs})
