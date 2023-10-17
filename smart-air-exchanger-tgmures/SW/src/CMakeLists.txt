# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*;
                              ${CMAKE_SOURCE_DIR}/lib/GPIO/src/*.*;
                              ${CMAKE_SOURCE_DIR}/lib/DS18B20/src/*.*;
                              ${CMAKE_SOURCE_DIR}/lib/PWM/src/*.*;
                              ${CMAKE_SOURCE_DIR}/lib/WIFI/src/*.*;
                              ${CMAKE_SOURCE_DIR}/lib/TASK/src/*.*;
                              )
idf_component_register(SRCS ${app_sources})
