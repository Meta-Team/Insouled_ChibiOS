set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 3.8)
project(Insouled_ChibiOS)

add_custom_target(STLink COMMAND make upload -C ${Insouled_ChibiOS_SOURCE_DIR}/dev
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})