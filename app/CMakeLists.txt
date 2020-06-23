set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_executable(qt_mpd_client_demo
  main.cpp
  mainwindow.cpp
  mainwindow.h
  demo.qrc
)


target_link_libraries(qt_mpd_client_demo PRIVATE Controller Qt5::Widgets PkgConfig::LIBMPDCLIENT)
target_include_directories(qt_mpd_client_demo PUBLIC "../Controller" ${LIBMPDCLIENT_INCLUDE_DIRS})

