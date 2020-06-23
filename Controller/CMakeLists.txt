add_library(Controller SHARED
  controller.cpp
  itemmodelcontroller.cpp
  tagitem.cpp
)

target_include_directories(Controller PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(Controller PRIVATE Qt5::Network PkgConfig::LIBMPDCLIENT)
target_compile_definitions(Controller PRIVATE CONTROLLER_LIBRARY)
