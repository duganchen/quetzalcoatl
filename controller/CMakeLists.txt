add_library(controller SHARED
  albumsitem.cpp
  albumitem.cpp
  artistsitem.cpp
  artistalbumitem.cpp
  artistitem.cpp
  artistsongsitem.cpp
  compilationsitem.cpp
  compilationartistitem.cpp
  compilationartistalbumitem.cpp
  controller.cpp
  composersitem.cpp
  composeritem.cpp
  composeralbumitem.cpp
  composersongsitem.cpp
  databasemodel.cpp
  directoryitem.cpp
  genreartistitem.cpp
  genreitem.cpp
  genreartistalbumitem.cpp
  genreartistsongsitem.cpp
  genresitem.cpp
  genresongsitem.cpp
  item.cpp
  itemmodel.cpp
  playlistmodel.cpp
  playlistsitem.cpp
  playlistitem.cpp
  playlistitem.cpp
  playlistsongitem.cpp
  queueditem.cpp
  songitem.cpp
  songsitem.cpp
  strformats.cpp
)

target_include_directories(controller PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(controller PRIVATE Qt5::Network Qt5::Gui PkgConfig::LIBMPDCLIENT)
target_compile_definitions(controller PRIVATE CONTROLLER_LIBRARY)
