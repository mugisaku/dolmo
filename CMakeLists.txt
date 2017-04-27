cmake_minimum_required(VERSION 2.8)
project(gmbb)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "CCACHE_DIR=/tmp/ccachedir ccache")
endif(CCACHE_FOUND)


set(CMAKE_VERBOSE_MAKEFILE ON)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_C_COMPILER  gcc)
set(CMAKE_C_FLAGS   "-std=c14 -Werror")
set(CMAKE_CXX_COMPILER  g++)
set(CMAKE_CXX_FLAGS "-std=c++14 -Werror")
set(CMAKE_CXX_LINK_FLAGS  "-lstdc++ -lz -lpng -lSDL2 -lSDL2main")


set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS}   -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS}   -Og -g")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -Og -g")


if(EMSCRIPTEN)
  set(EXECUTABLE_EXTENSION ".html")
  set(CMAKE_CXX_COMPILER  em++)
  set(CMAKE_CXX_FLAGS "-std=c++14 -Werror -Wno-switch -O2")
  set(CMAKE_CXX_FLAGS_DEBUG "")
  set(CMAKE_CXX_LINK_FLAGS  "${CMAKE_CXX_LINK_FLAGS} -s USE_LIBPNG=1 -s USE_SDL=2 --preload-file dolmo_parts.png  --use-preload-plugins")
endif()




set(SOURCES
  dolmo_rectangle.cpp
  dolmo_image.cpp
  dolmo_renderer.cpp
  dolmo_gui.cpp
  dolmo_screen.cpp
  dolmo_font.cpp
  dolmo_node.cpp
  dolmo_node__table.cpp
  dolmo_human_model.cpp
  dolmo_horse_model.cpp
  dolmo_doll.cpp
  dolmo_dollState.cpp
  dolmo_frame.cpp
  dolmo_scene.cpp
  dolmo_sceneEditor.cpp
  dolmo_sceneEditor__callback.cpp
  dolmo_sceneEditor__save.cpp
  libjson/json_stream.cpp
  libjson/json_stream__scan.cpp
  libjson/json_value.cpp
)


include_directories(.)
include_directories(./libjson)
include_directories(/usr/include/SDL2)
add_executable("Dolmo${EXECUTABLE_EXTENSION}" dolmo.cpp ${SOURCES})


if(EMSCRIPTEN)
 set_target_properties("Dolmo${EXECUTABLE_EXTENSION}" PROPERTIES LINK_FLAGS "--shell-file shell.html")
endif()




