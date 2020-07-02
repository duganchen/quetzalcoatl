add_library(Controller SHARED
  icons.qrc
  item.cpp
  itemmodelcontroller.cpp
  controller.cpp
)

target_include_directories(Controller PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(Controller PRIVATE Qt5::Network Qt5::Gui PkgConfig::LIBMPDCLIENT)
target_compile_definitions(Controller PRIVATE CONTROLLER_LIBRARY)
