#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <algorithm>
#include <sstream>
#include <signal.h>
#include "network/TcpServer.hpp"
#include "common/codec.h"
#include "common/SudokuBoard.hpp"

using namespace SimpleNet;

SudokuBoard board;
std::vector<Socket*> clients;
std::mutex clientsMutex;
Codec codec;

void broadcast(const std::string& data) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto it = clients.begin(); it != clients.end(); ) {
        try {
            (*it)->send(data);
            ++it;
        } catch (...) {
            it = clients.erase(it);
        }
    }
}

void signalHandler(int signum) {
    std::cout << "\n[SERVER] Shutting down" << std::endl;
    exit(signum);
}

void mainHandler(Socket clientSocket) {
    Socket* ptr = &clientSocket;
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.push_back(ptr);
    }
    
    std::cout << "[SERVER] New client connected. Total clients: " << clients.size() << std::endl;
    
    clientSocket.send(codec.encode({board.toString()}));

    try {
        while (true) {
            auto data = clientSocket.receive();
            if (data.empty()) break;
            
            std::string msg(data.begin(), data.end());
            std::vector<std::string> cmds = codec.decode(msg);
            
            for (const auto& cmd_str : cmds) {
                std::stringstream ss(cmd_str);
                std::string action; 
                ss >> action;
                
                if (action == "PUT") {
                    int r, c, v;
                    if (ss >> r >> c >> v) {
                        std::cout << "[MOVE] Client tries: Row=" << r << " Col=" << c << " Val=" << v << std::endl;

                        if (board.isValid(r - 1, c - 1, v)) {
                            board.setCell(r - 1, c - 1, v);
                            broadcast(codec.encode({board.toString()}));
                            
                            if (board.isFull()) {
				    std::cout << "[GAME OVER] Board is full" << std::endl;    
                                broadcast(codec.encode({"WIN"}));
                            }
                        } else {
                            std::cout << "[REJECTED] Move is invalid" << std::endl;
                            clientSocket.send(codec.encode({"ERROR"}));
                        }
                    }
                }
            }
        }
    } catch (...) {}

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.erase(std::remove(clients.begin(), clients.end(), ptr), clients.end());
    }
    std::cout << "[SERVER] Client disconnected" << clients.size() << std::endl;
}

int main() {
    signal(SIGINT, signalHandler);
    
    std::cout << "[SERVER] Generating new Sudoku puzzle..." << std::endl;
    board.initializeRandom(40);
    
    std::cout << "[SERVER] Waiting for players..." << std::endl;
    
    try {
        TcpServer server(1234, 4);
        server.run(mainHandler);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
