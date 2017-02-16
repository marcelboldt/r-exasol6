
include_directories(../../rapidjson ../../websockets/websockets)
add_subdirectory(../../websockets/websockets CMAKE_BINARY_DIR)


find_package(OpenSSL REQUIRED)

set(SOURCE_FILES
        exasockets.h exasockets.cpp
        osname.h
       exaResultSet.h exaResultSet.cpp
        )
add_library(exasockets ${SOURCE_FILES})

set(LINK_LIBS
        websockets
        ${OPENSSL_LIBRARIES}
        )
target_link_libraries(exasockets ${LINK_LIBS})