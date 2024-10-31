message(WARNING "maxxu into cmake")
cmake_minimum_required(VERSION 3.30.5)
project(bytehook)

enable_language(ASM)

if((${ANDROID_ABI} STREQUAL "arm64-v8a") OR (${ANDROID_ABI} STREQUAL "x86_64"))
    set(ARCH_LINK_FLAGS "-Wl,-z,max-page-size=16384")
else()
    set(ARCH_LINK_FLAGS "")
endif()

file(GLOB SRC *.c)
add_library(bytehook SHARED ${SRC})
target_compile_features(bytehook PUBLIC c_std_17)
target_compile_options(bytehook PUBLIC -std=c17 -Weverything -Werror)
target_include_directories(bytehook PUBLIC . include third_party/bsd third_party/lss)
target_link_libraries(bytehook log)

if(USEASAN)
    target_compile_options(bytehook PUBLIC -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(bytehook PUBLIC -fsanitize=address)
else()
    target_compile_options(bytehook PUBLIC -Oz -ffunction-sections -fdata-sections)
    target_link_options(bytehook PUBLIC -Oz -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/bytehook.map.txt)
endif()
#
#if((${ANDROID_ABI} STREQUAL "arm64-v8a") OR (${ANDROID_ABI} STREQUAL "armeabi-v7a"))
#    set(ROOT_PATHS ${CMAKE_FIND_ROOT_PATH})
#    foreach(PATH IN LISTS ROOT_PATHS)
#        string(STRIP "${PATH}" STRIPPED_PATH)
#        if(STRIPPED_PATH MATCHES ".*prefab/${ANDROID_ABI}.*")
#            set(PREFAB_PATH "${STRIPPED_PATH}")
#            break()
#        endif()
#    endforeach()
#
#    if(NOT DEFINED PREFAB_PATH)
#        message(FATAL_ERROR "Could not find prefab path in CMAKE_FIND_ROOT_PATH")
#    endif()
#    message(WARNING "PREFAB_PATH ${PREFAB_PATH}")
#    find_file(SHADOWHOOK_CONFIG
#            NAME "shadowhookConfig.cmake"
#            PATHS "${PREFAB_PATH}/lib/**/shadowhook"
#            NO_DEFAULT_PATH
#            NO_CMAKE_FIND_ROOT_PATH)
#
#    if(SHADOWHOOK_CONFIG)
#        include(${SHADOWHOOK_CONFIG})
#        target_link_libraries(bytehook shadowhook::shadowhook)
#    else()
#        message(FATAL_ERROR "Could not find shadowhook configuration file")
#    endif()
#endif()

find_package(shadowhook REQUIRED CONFIG)
target_link_libraries(bytehook shadowhook::shadowhook)