add_library(controller SHARED
  albumitem.cpp
  albumsitem.cpp
  artistalbumitem.cpp
  artistitem.cpp
  artistsitem.cpp
  artistsongsitem.cpp
  compilationartistalbumitem.cpp
  compilationartistitem.cpp
  compilationsitem.cpp
  composeralbumitem.cpp
  composeritem.cpp
  composersitem.cpp
  composersongsitem.cpp
  controller.cpp
  databasemodel.cpp
  directoryitem.cpp
  genreartistalbumitem.cpp
  genreartistitem.cpp
  genreartistsongsitem.cpp
  genreitem.cpp
  genresitem.cpp
  genresongsitem.cpp
  item.cpp
  itemmodel.cpp
  orderedentitysongitem.cpp
  orderedmpdsongitem.cpp
  orderedsongitem.cpp
  playlistitem.cpp
  playlistsitem.cpp
  queueditem.cpp
  queuemodel.cpp
  songitem.cpp
  songsitem.cpp
  strformats.cpp
  unorderedentitysongitem.cpp
  unorderedmpdsongitem.cpp
  unorderedsongitem.cpp
)

target_include_directories(controller PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(controller PRIVATE Qt5::Network Qt5::Gui PkgConfig::LIBMPDCLIENT)
target_compile_definitions(controller PRIVATE CONTROLLER_LIBRARY)
