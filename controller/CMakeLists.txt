add_library(controller SHARED
  item.cpp
  genresitem.cpp
  icons.qrc
  dbitem.cpp
  items.cpp
  controller.cpp
  timeformat.cpp
  songitem.cpp
)

target_include_directories(controller PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(controller PRIVATE Qt5::Network Qt5::Gui PkgConfig::LIBMPDCLIENT)
target_compile_definitions(controller PRIVATE CONTROLLER_LIBRARY)
