project(LUT)

add_compile_options(
       "-Wall"
       "$<$<CONFIG:RELEASE>:-O3>"
       "$<$<CONFIG:DEBUG>:-g>"
)

set(INCLUDES    /
)

set(SOURCES lut.c
)

add_executable(LUT ${SOURCES})

target_include_directories(LUT PRIVATE ${INCLUDES})
