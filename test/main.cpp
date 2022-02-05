#include "config.hpp"

#include <catch2/catch_session.hpp>
#include <iostream>
#include <string>

std::string http_address, ws_address, wss_address;
int http_port{}, ws_port{}, wss_port{};
bool run_http_tests{}, run_ws_tests{}, run_wss_tests{};

auto set_endpoint(std::string& address, int& port, bool& trigger, std::string const& provided)
{
    try
    {
        auto port_sep{provided.find_last_of(':')};
        port = std::stoi(provided.substr(port_sep + 1));
        address = provided.substr(0, port_sep);
        trigger = true;

        // std::cout << "Running tests against node @ " << address << ", port=" << port << std::endl;
        return true;
    }
    catch (...)
    {
        std::cerr << "Invalid format in " << provided << std::endl;
    }
    return false;
}

int main(int argc, char* argv[])
{
    Catch::Session session; // There must be exactly one instance

    std::string http_node, ws_node, wss_node;

    using namespace Catch::Clara;
    auto cli = session.cli() | Opt(http_node, "http")["--http"]("http address (address:port)") |
               Opt(ws_node, "ws")["--ws"]("websocket address (address:port)") |
               Opt(wss_node, "wss")["--wss"]("secure websocket address (address:port)");

    session.cli(cli);

    if (auto err{session.applyCommandLine(argc, argv)})
    {
        return err;
    }

    if (!http_node.empty() && !set_endpoint(http_address, http_port, run_http_tests, http_node))
    {
        return -1;
    }
    if (!ws_node.empty() && !set_endpoint(ws_address, ws_port, run_ws_tests, ws_node))
    {
        return -1;
    }
    if (!wss_node.empty() && !set_endpoint(wss_address, wss_port, run_wss_tests, wss_node))
    {
        return -1;
    }

    // default to testing against a known ws-enabled node
    if (!(run_http_tests || run_ws_tests || run_wss_tests))
    {
        set_endpoint(ws_address, ws_port, run_ws_tests, "ws://chadasscapital.com:35998");
    }

    return session.run();
}
