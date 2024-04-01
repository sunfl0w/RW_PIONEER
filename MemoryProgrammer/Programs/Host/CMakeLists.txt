cmake_minimum_required(VERSION 3.9)

project(RW_FLASH VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(
       "-Wall"
       "$<$<CONFIG:RELEASE>:-O3>"
       "$<$<CONFIG:DEBUG>:-g>"
)

set(SOURCES "src/main.c")

add_executable(RW_FLASH ${SOURCES})

find_library(SERIAL serialport)
target_link_libraries(RW_FLASH PRIVATE ${SERIAL})
