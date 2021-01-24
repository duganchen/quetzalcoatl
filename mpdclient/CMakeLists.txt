add_library(mpdclient SHARED
  connection.cpp
  song.cpp
  entitysong.cpp
)

target_include_directories(mpdclient PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(mpdclient PRIVATE Qt5::Core PkgConfig::LIBMPDCLIENT)
target_compile_definitions(mpdclient PRIVATE MPDCLIENT_LIBRARY)
set_property(TARGET mpdclient PROPERTY CXX_STANDARD 20)
