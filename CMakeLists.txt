cmake_minimum_required(VERSION 3.10)

project(ZenonSDK VERSION 0.01 LANGUAGES CXX)

### cmake properties

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

### project version

configure_file(zenon_sdk_config.h.in zenon_sdk_config.h)

### subprojects and libraries

# needing these for secure sockets on macos
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(MACOS_CORE CoreFoundation)
    find_library(MACOS_SEC Security)
    set(SYS_LIBS ${MACOS_CORE} ${MACOS_SEC})
endif()

find_package(Threads REQUIRED)

set(THIRDPARTY "${PROJECT_SOURCE_DIR}/thirdparty")

# websocket client IXWebSocket
set(USE_TLS ON CACHE BOOL "Build IXWebSocket with TLS support")
#add_subdirectory("${THIRDPARTY}/IXWebSocket")
set(WS_INC "${THIRDPARTY}/IXWebSocket/")
#set(WS_LIB "${CMAKE_BUILD_DIR}/thirdparty/IXWebSocket/libixwebsocket.a")


#find_package(OpenSSL REQUIRED)

# unit test library catch2
add_subdirectory("${THIRDPARTY}/catch2")

# path to catch2 provided module files for automatic test discovery
list(APPEND CMAKE_MODULE_PATH "${THIRDPARTY}/catch2/extras")

### targets

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src/")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test/")

set(TEST_INCLUDES
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
    "${THIRDPARTY}"
    "${THIRDPARTY}/catch2/src"
    "${THIRDPARTY}/IXWebSocket/"
)

include(FetchContent)
FetchContent_Declare(ixwebsocket GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git)
FetchContent_MakeAvailable(ixwebsocket)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

set(TEST_LIBS Catch2::Catch2WithMain Catch2::Catch2 Threads::Threads "${SYS_LIBS}")#"${OPENSSL_LIBRARIES}")

set(API_SRC ${SRC_DIR}/api_embedded.cpp ${SRC_DIR}/api_ledger.cpp)

add_executable(embedded_tests "${PROJECT_SOURCE_DIR}/test/embedded.cpp" ${API_SRC} ${SRC_DIR}/sdk.cpp ${TEST_DIR}/connection.cpp)
target_include_directories(embedded_tests PUBLIC "${TEST_INCLUDES}")
target_link_libraries(embedded_tests PRIVATE "${TEST_LIBS}" ixwebsocket)

add_executable(connection_tests "${PROJECT_SOURCE_DIR}/test/connection.cpp" ${SRC_DIR}/sdk.cpp ${SRC_DIR}/api_ledger.cpp ${TEST_DIR}/connection.cpp)
target_include_directories(connection_tests PUBLIC "${TEST_INCLUDES}")
target_link_libraries(connection_tests "${TEST_LIBS}" ixwebsocket)

### cmake test discovery, only required if ctest is wanted

#include(CTest)
## provided by catch2 in /extras
#include(Catch)
#catch_discover_tests(embedded_tests)
