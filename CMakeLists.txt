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

### targets

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src/")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test/")

include(FetchContent)

set(USE_TLS ON CACHE INTERNAL "Enable secure websocket support")
FetchContent_Declare(ixwebsocket 
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG 5f2955ef78edde86f284257462d5ca03416696e3) # v11.3.2

FetchContent_MakeAvailable(ixwebsocket)

FetchContent_Declare(catch2 
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.0.0-preview4) #  f4af9f69265d009a457aa99d1075cfba78652a66

FetchContent_MakeAvailable(catch2)

FetchContent_Declare(jsonrpccxx 
    GIT_REPOSITORY https://github.com/jsonrpcx/json-rpc-cxx
    GIT_TAG 7bcd0d2e4e0b138558b019bb75a61b5bc7fe2380) # v0.3.0

FetchContent_MakeAvailable(jsonrpccxx)

set(TEST_INCLUDES
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
    "${catch2_SOURCE_DIR}/src"
    "${jsonrpccxx_SOURCE_DIR}/include"
    "${jsonrpccxx_SOURCE_DIR}/vendor"
)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

set(TEST_LIBS Catch2::Catch2 Threads::Threads "${SYS_LIBS}")

set(API_SRC ${SRC_DIR}/api_embedded.cpp ${SRC_DIR}/api_ledger.cpp)

add_executable(embedded_tests
    "${PROJECT_SOURCE_DIR}/test/embedded.cpp" "${PROJECT_SOURCE_DIR}/test/main.cpp"
    ${API_SRC} ${SRC_DIR}/sdk.cpp ${TEST_DIR}/connection.cpp)

target_include_directories(embedded_tests PUBLIC "${TEST_INCLUDES}")

target_link_libraries(embedded_tests PRIVATE "${TEST_LIBS}" ixwebsocket)

add_executable(connection_tests
    "${PROJECT_SOURCE_DIR}/test/connection.cpp" "${PROJECT_SOURCE_DIR}/test/main.cpp"
    ${SRC_DIR}/sdk.cpp ${SRC_DIR}/api_ledger.cpp ${TEST_DIR}/connection.cpp)

target_include_directories(connection_tests PUBLIC "${TEST_INCLUDES}")

target_link_libraries(connection_tests "${TEST_LIBS}" ixwebsocket)

### cmake test discovery

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras/)
include(CTest)
include(Catch)
catch_discover_tests(embedded_tests)
