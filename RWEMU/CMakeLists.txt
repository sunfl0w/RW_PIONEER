cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(
       "-Wall"
       "$<$<CONFIG:RELEASE>:-O3>"
       "$<$<CONFIG:DEBUG>:-g>"
)

SET(RWEMU_SRC
    src/main.c
)

SET(RWEMU_INCLUDEDIRS
)

project(RWEMU)

add_executable(RWEMU ${RWEMU_SRC})

target_include_directories(RWEMU PRIVATE ${RWEMU_INCLUDEDIRS})