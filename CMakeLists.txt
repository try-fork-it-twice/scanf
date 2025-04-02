cmake_minimum_required(VERSION 3.15)
project(example)

set(FREERTOS_KERNEL_PATH "./FreeRTOS-Kernel")

# Add the freertos_config for FreeRTOS-Kernel
add_library(freertos_config INTERFACE)

target_include_directories(freertos_config
    INTERFACE
    "./src"
    "./samples"
)

if (DEFINED FREERTOS_SMP_EXAMPLE AND FREERTOS_SMP_EXAMPLE STREQUAL "1")
    message(STATUS "Build FreeRTOS SMP example")
    add_compile_options( -DconfigNUMBER_OF_CORES=2 -DconfigUSE_PASSIVE_IDLE_HOOK=0 )
endif()

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

# Select the native compile PORT
set(FREERTOS_PORT "GCC_POSIX" CACHE STRING "" FORCE)

# Adding the FreeRTOS-Kernel subdirectory
add_subdirectory(${FREERTOS_KERNEL_PATH})

target_compile_options(freertos_kernel PRIVATE
    ### Gnu/Clang C Options
    $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wpedantic>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wconversion>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>

    # Suppressions required to build clean with clang.
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-unused-macros>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-padded>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-missing-variable-declarations>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-covered-switch-default>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-cast-align>
)

add_executable(${PROJECT_NAME}
    src/scanf.c
    samples/example.c
)

target_link_libraries(${PROJECT_NAME} freertos_kernel freertos_config)

set_property(TARGET freertos_kernel PROPERTY C_STANDARD 11)
