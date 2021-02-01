add_library(itemmodels SHARED
  itemmodel.cpp
  item.cpp
  albumsitem.cpp
)

target_link_libraries(itemmodels PRIVATE mpdclient Qt5::Gui)
target_include_directories(itemmodels PUBLIC "../mpdclient")
