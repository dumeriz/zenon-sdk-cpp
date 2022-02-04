# zenon-sdk-cpp
C++ implementation of the Zenon Network Wallet SDK.

Currently only the basic structure and some wrappers for the JSON api exist.

### Usage
On macOS, install the required libraries for (secure) websocket transport using homebrew (deflate optional):
```
brew install openssl libdeflate
```
Then build the tests:
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
by the user via `sdk::set_connector`. HTTP-, WS- and WSS-connectors are implemented in the corresponding header files.
See the various test units for examples.

Testing framework is Catch2, https://github.com/catchorg/Catch2

Jsonrpc library is json-rpc-cxx, https://github.com/jsonrpcx/json-rpc-cxx/tree/master/include/jsonrpccxx

Websocket library is IXWebSocket, https://github.com/machinezone/IXWebSocket
