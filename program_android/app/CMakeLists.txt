cmake_minimum_required(VERSION 3.4.1)

## NOTE: these paths are different for different Android Studio versions. the followed starts from 3.6 .
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(built_jni_subpath build/intermediates/library_jni/release/jni/${ANDROID_ABI})
else()
    set(built_jni_subpath build/intermediates/library_jni/debug/jni/${ANDROID_ABI})
endif()

function(get_absolute_paths paths)
    foreach(it ${${paths}})
        list(APPEND abs_paths ${CMAKE_CURRENT_SOURCE_DIR}/${it})
    endforeach()
    set(${paths} ${abs_paths} PARENT_SCOPE)
endfunction()

### configuration begin
set(lib_name cargo)

list(APPEND inc_path ../../GHEADERS)

list(APPEND src_aims ../../mainsrc/craise/*.c)
list(APPEND src_aims ../../mainsrc/main/*.c)
list(APPEND src_aims ../../mainsrc/uidev/uidev/*.c)
list(APPEND src_aims ../../mainsrc/uidev/uidev_android/*.c)

list(APPEND sys_libs log)
### configutation end

include_directories(${inc_path})

file(GLOB_RECURSE src_files ${src_aims})
add_library(${lib_name} SHARED ${src_files})

get_absolute_paths(usr_libs)
target_link_libraries(${lib_name} ${sys_libs} ${usr_libs})
