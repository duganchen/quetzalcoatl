file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(connectionTest tst_testconnection.cpp mpdprocess.cpp)
add_test(NAME connectionTest COMMAND connectionTest)

target_include_directories(connectionTest PRIVATE Qt5::Test "${PROJECT_SOURCE_DIR}/Controller" ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(connectionTest PRIVATE Controller PkgConfig::LIBMPDCLIENT Qt5::Test)

add_executable(controllerTest tst_testcontroller.cpp mpdprocess.cpp)
add_test(NAME controllerTest COMMAND controllerTest)

target_include_directories(controllerTest PRIVATE Qt5::Test "${PROJECT_SOURCE_DIR}/Controller" ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(controllerTest PRIVATE Controller PkgConfig::LIBMPDCLIENT Qt5::Test)
