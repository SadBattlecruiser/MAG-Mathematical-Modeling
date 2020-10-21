cmake_minimum_required(VERSION 3.18)

project(lab2)

set(PROJECT_SOURCE_DIR C:/Учеба/Mathematical-Modeling/lab2/src)
set(PROJECT_OUT_DIR C:/Учеба/Mathematical-Modeling/lab2/out)
include_directories(C:/Учеба/Mathematical-Modeling/lab2/src)

#set(SOURCE_EXE src\\main.cpp)

#set(SOURCE_LIB src\\euler_plugin.cpp src\\heun_plugin.cpp src\\to_console_plugin.cpp
#src\\txt_config_plugin.cpp src\\to_ftp_plugin.cpp src\\to_csv_plugin.cpp)

add_library(euler_plugin.plg SHARED ${PROJECT_SOURCE_DIR}/euler_plugin.cpp)
add_library(heun_plugin.plg SHARED ${PROJECT_SOURCE_DIR}/heun_plugin.cpp)
add_library(to_console_plugin.plg SHARED ${PROJECT_SOURCE_DIR}/to_console_plugin.cpp)
add_library(txt_config_plugin.plg SHARED ${PROJECT_SOURCE_DIR}/txt_config_plugin.cpp)
add_library(to_csv_plugin.plg SHARED ${PROJECT_SOURCE_DIR}/to_csv_plugin.cpp)
add_library(to_ftp_plugin.plg SHARED ${PROJECT_SOURCE_DIR}/to_ftp_plugin.cpp)

add_executable(main.exe ${PROJECT_SOURCE_DIR}/main.cpp)
