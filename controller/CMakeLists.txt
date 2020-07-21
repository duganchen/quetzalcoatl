add_library(controller SHARED
  controller.cpp
  databasemodel.cpp
  dbitem.cpp
  dbsongitem.cpp
  genreitem.cpp
  genresitem.cpp
  genresongsitem.cpp
  item.cpp
  itemmodel.cpp
  playlistmodel.cpp
  songitem.cpp
  timeformat.cpp
)

target_include_directories(controller PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(controller PRIVATE Qt5::Network Qt5::Gui PkgConfig::LIBMPDCLIENT)
target_compile_definitions(controller PRIVATE CONTROLLER_LIBRARY)
