#pragma once

#include <chrono>
#include <jsonrpccxx/common.hpp>
#include <jsonrpccxx/iclientconnector.hpp>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <websocketpp/client.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

using namespace std::chrono_literals;

namespace sdk
{

    class ws_connector : public jsonrpccxx::IClientConnector
    {
        using client_t  = websocketpp::client<websocketpp::config::asio_client>;
        using message_t = websocketpp::config::asio_client::message_type::ptr;

    public:
        ws_connector(std::string const& host, uint16_t port)
        {
            client_.clear_access_channels(websocketpp::log::alevel::frame_header);
            client_.clear_access_channels(websocketpp::log::alevel::frame_payload);

            client_.init_asio();

            client_.set_open_handler([&](auto handle) { on_open(handle); });
            client_.set_fail_handler([&](auto handle) { on_fail(handle); });
            client_.set_message_handler([&](auto handle, auto message) { on_message(handle, message); });
            client_.set_close_handler([&](auto handle) { on_close(handle); });

            websocketpp::lib::error_code ec;
            auto endpoint   = host + ":" + std::to_string(port);
            auto connection = client_.get_connection(endpoint, ec);

            if (ec)
            {
                throw std::runtime_error{"get_connection error " + ec.message()};
            }

            handle_ = connection->get_handle();

            // queued, connection is established in the run_thread_
            client_.connect(connection);

            run_thread_ = websocketpp::lib::thread(&client_t::run, &client_);

            auto timeout{std::chrono::system_clock::now() + 1s};
            while (!connected_.load() && std::chrono::system_clock::now() < timeout)
            {
                std::this_thread::sleep_for(10ms);
            }

            if (!connected_.load())
            {
                throw std::runtime_error{"Failed to connect to " + endpoint};
            }
        }

        ~ws_connector()
        {
            if (connected_)
            {
                client_.close(handle_, websocketpp::close::status::normal, "");
            }
            run_thread_.join();
        }

        auto Send(std::string const& message) -> std::string override
        {
            std::string response;
            if (awaiting_msg_.exchange(true)) // parallel calls not supported in sync message
            {
                return response; // define a unique error message for Send failure
            }

            client_.send(handle_, message, websocketpp::frame::opcode::text);

            auto timeout = std::chrono::system_clock::now() + 1s;
            while (awaiting_msg_.load() && std::chrono::system_clock::now() < timeout)
            {
                std::this_thread::sleep_for(10ms);
            }

            if (awaiting_msg_.load())
            {
                return ""; // define error for timeout
            }

            {
                std::scoped_lock lock(mutex_);
                std::swap(response, response_);
            }
            return response;
        }

    private:
        client_t client_;
        websocketpp::connection_hdl handle_;

        std::atomic_bool connected_{false};
        std::atomic_bool awaiting_msg_{false};

        std::mutex mutex_; // locking response_
        std::string response_;

        websocketpp::lib::thread run_thread_; // connection active in the background

        auto on_open(websocketpp::connection_hdl handle) -> void
        {
            connected_.store(true);
            // logging?
        }

        void on_fail(websocketpp::connection_hdl) { connected_.store(false); }

        void on_message(websocketpp::connection_hdl hdl, message_t msg)
        {
            std::scoped_lock lock(mutex_);
            response_ = msg->get_payload();
            awaiting_msg_.store(false);
        }

        void on_close(websocketpp::connection_hdl hdl) { connected_.store(false); }
    };
} // namespace sdk
