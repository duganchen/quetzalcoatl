set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_executable(quetzalcoatl
  configurationdialog.cpp
  main.cpp
  mainwindow.cpp
  mainwindow.h
  icons.qrc
)


target_link_libraries(quetzalcoatl PRIVATE Controller Qt5::Widgets PkgConfig::LIBMPDCLIENT)
target_include_directories(quetzalcoatl PUBLIC "../Controller" ${LIBMPDCLIENT_INCLUDE_DIRS})

