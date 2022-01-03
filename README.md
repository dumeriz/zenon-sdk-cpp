# zenon-sdk-cpp
C++ implementation of the Zenon Network Wallet SDK.

Currently only the basic structure and some wrappers for the JSON api exist.

### Usage
```
git submodule update --init --recursive
mkdir build && cd build
cmake .. && make
```
should build the target `embedded_tests`.
There is no other target yet.

```
./embedded_tests
```
should execute the only available test, which retrieves the current QSR registration cost from the api
with both a http- and a websocket connection.

Concept:

API-method wrappers should function independently of the connection type. The connection type must be set
by the user via `sdk::set_connector`. HTTP- and WS-connectors are implemented in the corresponding header files.

Those probably also work with the secure endpoints.

Async variants for both connectors should also be added.

Testing framework is Catch2, https://github.com/catchorg/Catch2

Jsonrpc library is json-rpc-cxx, https://github.com/jsonrpcx/json-rpc-cxx/tree/master/include/jsonrpccxx

Websocket library is websocketpp, https://github.com/zaphoyd/websocketpp
