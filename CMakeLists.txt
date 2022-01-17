# lua-timerfd: timerfd for lua
# Jian Chen <admin@chensoft.com>
# http://chensoft.com
cmake_minimum_required(VERSION 3.1)
project(timerfd VERSION 1.0.1 LANGUAGES C)

add_library(timerfd MODULE "")

if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    message(FATAL_ERROR "timerfd is only valid under Linux")
endif()

# environment
set_target_properties(timerfd PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
set_target_properties(timerfd PROPERTIES PREFIX "")

# source codes
file(GLOB PROJ_INC src/*.h)
file(GLOB PROJ_SRC src/*.c)

# third party
find_package(Lua)

# generate lib
target_include_directories(timerfd PRIVATE ${LUA_INCLUDE_DIR})
target_sources(timerfd PRIVATE ${PROJ_INC} ${PROJ_SRC})
target_link_libraries(timerfd ${LUA_LIBRARIES})

# install lib
install(TARGETS timerfd DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
