cmake_minimum_required(VERSION 3.6)
project(quetzalcoatl LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Widgets Gui Network)
if (NOT Qt6_FOUND)
find_package(
  Qt5 5.15
  COMPONENTS Core Widgets Gui Network
  REQUIRED)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMPDCLIENT REQUIRED IMPORTED_TARGET libmpdclient)

add_subdirectory(controller)
add_subdirectory(app)
