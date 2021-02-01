add_library(models SHARED
  itemmodel.cpp
  item.cpp
)

target_link_libraries(models PRIVATE mpdclient Qt5::Gui)
