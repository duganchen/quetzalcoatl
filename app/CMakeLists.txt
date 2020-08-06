set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_executable(quetzalcoatl
  connectiondialog.cpp
  main.cpp
  mainwindow.cpp
  mainwindow.h
  playlistdelegate.cpp
  playbacksettingsdialog.cpp
  resources.qrc
  saveplaylistdialog.cpp
)


target_link_libraries(quetzalcoatl PRIVATE controller Qt5::Widgets PkgConfig::LIBMPDCLIENT)
target_include_directories(quetzalcoatl PUBLIC "../controller" ${LIBMPDCLIENT_INCLUDE_DIRS})

