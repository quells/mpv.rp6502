cmake_minimum_required(VERSION 3.13)

project(MY-RP6502-PROJECT C ASM)

add_subdirectory(rp6502-sdk)

add_executable(mpv)
rp6502_executable(mpv)
target_sources(mpv PRIVATE
    src/mpv.c
)
target_link_libraries(mpv PRIVATE
    rp6502
)
