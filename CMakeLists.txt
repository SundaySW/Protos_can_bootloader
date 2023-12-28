cmake_minimum_required(VERSION 3.16)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to DEBUG")
    set(CMAKE_BUILD_TYPE DEBUG)
else()
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

project(sa_pico_can_BootLoader     VERSION 1.0     LANGUAGES   C CXX ASM)
set(DEVICE_FAMILY        STM32G4xx)
set(DEVICE_NAME          STM32G431xx)
set(DEVICE_FULL_NAME     STM32G431KBTx)

include(cmake/utils.cmake)
include(FetchContent)

##FetchContent_Populate(
#FetchContent_Declare(
#        sa_stm32_g4
#        GIT_REPOSITORY https://github.com/reus-inc/sa_stm32_g4.git
#        GIT_TAG main
#        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libs/sa_stm32_g4
#)
##FetchContent_Populate(
#FetchContent_Declare(
#        protos_core
#        GIT_REPOSITORY https://github.com/reus-inc/protos_core.git
#        GIT_TAG main
#        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libs/protos_core
#)
#if(NOT sa_stm32_g4_POPULATED)
#    FetchContent_Populate(sa_stm32_g4)
#endif()
#if(NOT protos_core_POPULATED)
#    FetchContent_Populate(protos_core)
#endif()

add_subdirectory(Drivers)

include_directories(
        App
        Core
        Drivers
        Libs
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.*
        ${CMAKE_CURRENT_SOURCE_DIR}/App/*.*
        ${CMAKE_CURRENT_SOURCE_DIR}/Libs/*.*
        )

add_executable(${PROJECT_NAME} startup_stm32g431xx.s)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/App
        ${CMAKE_CURRENT_SOURCE_DIR}/Libs
        )

target_sources(${PROJECT_NAME}
        PUBLIC
        ${SOURCES}
        )

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        SUFFIX ".elf"
        C_STANDARD 11
        C_EXTENSIONS ON
        CMAKE_C_STANDARD_REQUIRED ON
        CXX_STANDARD 17
        CXX_EXTENSIONS ON
        CMAKE_CXX_STANDARD_REQUIRED ON
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_NAME}-Drivers
        )

target_link_options(${PROJECT_NAME}
        PUBLIC
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map,--cref
        )

utils_target_set_linker_script(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/${DEVICE_FULL_NAME}_FLASH.ld
        )

utils_target_generate_hex(${PROJECT_NAME})
utils_target_generate_bin(${PROJECT_NAME})
utils_target_print_size(${PROJECT_NAME})

#target_compile_options(${PROJECT_NAME}
#        PRIVATE
#        -Wall
#        -Wextra
#        -Wpedantic
#        -Werror
#        -Wduplicated-cond
#        -Wduplicated-branches
#        -Wlogical-op
#        -Wnull-dereference
#        -Wshadow
#        -Wconversion
#        -Wsign-conversion
#        )

#if(RTT_LOG)
#    add_subdirectory(libs/rtt)
#    target_link_libraries(${PROJECT_NAME}
#            PRIVATE
#            rtt
#)
#endif()