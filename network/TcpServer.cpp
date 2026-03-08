#include "TcpServer.hpp"
#include <sys/epoll.h>
#include <iostream>
#include <memory>
#include <vector>

namespace SimpleNet {

TcpServer::TcpServer(int port, size_t threads) : pool_(threads) {
    listen_socket_.bind(port);
    listen_socket_.listen();
}
void TcpServer::run(ClientHandler handler) {
    int epollfd = epoll_create1(0);
    if (epollfd == -1) {
        throw std::runtime_error("Failed to create epoll");
    }
    epoll_event ev;
    ev.events = EPOLLIN; 
    ev.data.fd = listen_socket_.get_fd();
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_socket_.get_fd(), &ev) == -1) {
        throw std::runtime_error("Failed to add listen socket to epoll");
    }

    epoll_event events[10]; 
	std::cout << "Server started with epoll..." << std::endl;
	while (true) {
        int nfds = epoll_wait(epollfd, events, 10, -1);
        
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_socket_.get_fd()) {
                Socket client = listen_socket_.accept();
                auto client_ptr = std::make_shared<Socket>(std::move(client));
                pool_.enqueue([handler, client_ptr]() {
                    handler(std::move(*client_ptr));
                });
            }
        }
    }
}

}
