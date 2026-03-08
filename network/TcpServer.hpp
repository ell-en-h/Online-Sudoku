#pragma once

#include "Socket.hpp"
#include <functional>
#include "ThreadPool.hpp"
namespace SimpleNet {

using ClientHandler = std::function<void(Socket)>;

class TcpServer {
public:
    TcpServer(int port, size_t threads);
    void run(ClientHandler handler);

private:
    Socket listen_socket_;
    ThreadPool pool_;
};

}

