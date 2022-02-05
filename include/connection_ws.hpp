#pragma once

#include "connection_error.hpp"
#include "connector.hpp"
#include "ixwebsocket/IXWebSocketMessage.h"
#include "ixwebsocket/IXWebSocketMessageType.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXUserAgent.h>
#include <ixwebsocket/IXWebSocket.h>

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;

namespace sdk
{
    class ws_connector : public connector
    {
    public:
        ws_connector(std::string const& host, uint16_t port)
        {
            // must do this on windows. To check: is it safe to call this repeatedly, or does it have to be encapsulated
            // in some static context? ix::initNetSystem();
            websocket_.setUrl(host + ":" + std::to_string(port));
            websocket_.setOnMessageCallback([&](auto const& msg) { delegate(msg); });
            if (auto result{websocket_.connect(2)}; !result.success)
            {
                throw connection_error{"Connection failed: " + result.errorStr};
            }
            websocket_.start();
        }

        ~ws_connector() {}

        /// @brief Send `message` to the Node and receive the response.
        ///
        /// Always call connected() before this, as this method does not check it.
        /// This message is explicitly NOT thread safe. You can call it from whatever thread you want,
        /// but if you call it concurrently from several threads, at best the request/response-pairs will
        /// be confused.
        ///
        /// @param message is the data to send.
        /// @return the response.
        auto Send(std::string const& message) -> std::string override
        {
            std::unique_lock<std::mutex> lock(req_res_mtx_);

            msg_received_.store(false);
            websocket_.send(message);

            if (response_trigger_.wait_for(lock, 5s, [&] { return msg_received_.load(); }))
            {
                return response_;
            }

            throw connection_error{"Timeout during receive"};
        }

        /// @brief Checks if a connection to the Node is established.
        auto connected() const -> bool override { return connected_.load(); }

    private:
        ix::WebSocket websocket_;

        std::mutex send_mutex_;
        std::mutex req_res_mtx_;
        std::condition_variable response_trigger_;
        std::atomic_bool connected_{false};
        std::atomic_bool msg_received_{false};

        std::mutex mutex_; // locking response_
        std::string response_;

        auto signal_response_received()
        {
            msg_received_.store(true);
            response_trigger_.notify_one();
        }

        auto on_open() { connected_.store(true); }

        auto on_close() { connected_.store(false); }

        auto on_message(ix::WebSocketMessagePtr const& msg)
        {
            {
                std::lock_guard<std::mutex> lock{req_res_mtx_};
                response_ = msg->str;
            }
            signal_response_received();
        }

        auto on_error(ix::WebSocketMessagePtr const& msg) const { throw connection_error{msg->errorInfo.reason}; }

        auto delegate(ix::WebSocketMessagePtr const& msg) -> void
        {
            switch (msg->type)
            {
            case ix::WebSocketMessageType::Open:
                on_open();
                break;
            case ix::WebSocketMessageType::Close:
                on_close();
                break;
            case ix::WebSocketMessageType::Error:
                on_error(msg);
                break;
            case ix::WebSocketMessageType::Message:
                on_message(msg);
            case ix::WebSocketMessageType::Fragment:
            case ix::WebSocketMessageType::Ping:
            case ix::WebSocketMessageType::Pong:
                break;
            }
        }
    };
} // namespace sdk
