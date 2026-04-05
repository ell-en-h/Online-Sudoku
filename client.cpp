#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <map>
#include <unistd.h>
#include "network/Socket.hpp"
#include "common/codec.h"

using namespace SimpleNet;

void displayBoard(const std::string& b) {
    if (b.length() != 81) return;

    std::cout << "\033[2J\033[H"; 
    std::cout << "    1 2 3   4 5 6   7 8 9" << std::endl;
    std::cout << "  +-------+-------+-------+" << std::endl;
    
    std::map<int, int> counts;
    for (int n = 1; n <= 9; ++n) counts[n] = 9;

    for (int i = 0; i < 9; ++i) {
        if (i > 0 && i % 3 == 0) 
            std::cout << "  +-------+-------+-------+" << std::endl;
        
        std::cout << (i + 1) << " | ";
        for (int j = 0; j < 9; ++j) {
            char c = b[i * 9 + j];
            if (c != '0') {
                std::cout << c << " ";
                counts[c - '0']--;
            } else {
                std::cout << ". ";
            }
            if ((j + 1) % 3 == 0) std::cout << "| ";
        }
        std::cout << std::endl;
    }
    std::cout << "  +-------+-------+-------+" << std::endl;
    
    for (int n = 1; n <= 9; ++n) {
        std::cout << n << ":[" << (counts[n] < 0 ? 0 : counts[n]) << "] ";
        if (n == 9) std::cout << std::endl;
    }
}

void receiveHandler(Socket& sock, Codec& codec) {
    try {
        while (true) {
            auto data = sock.receive();
            if (data.empty()) {
                std::cout << "\n\033[1;33m[DISCONNECTED] Server closed.\033[0m" << std::endl;
                _exit(0); 
            }
            
            std::vector<std::string> messages = codec.decode(std::string(data.begin(), data.end()));
            for (const auto& msg : messages) {
                if (msg.length() == 81) {
                    displayBoard(msg);
                } else if (msg == "WIN") {
                    std::cout << "\n\033[1;32m*** SUDOKU SOLVED! ***\033[0m\n" << std::endl;
                    _exit(0);
                } else if (msg == "ERROR") {
                    std::cout << "\n\033[1;31m>> INVALID MOVE!<<\033[0m" << std::endl;
		}
            }
        }
    } catch (...) {
        _exit(0);
    }
}

int main() {
    Socket s; 
    Codec c;
    
    try { 
        s.connect("127.0.0.1", 1234); 
    } catch (...) { 
        std::cerr << "[ERROR] Cannot connect to server" << std::endl;
        return 1; 
    }

    std::thread(receiveHandler, std::ref(s), std::ref(c)).detach();

    std::string inputLine;
    while (std::getline(std::cin, inputLine)) {
        if (inputLine.empty()) continue;
        
        std::stringstream ss(inputLine); 
        int row, col, val;
        if (ss >> row >> col >> val) {
            std::string cmd = "PUT " + std::to_string(row) + " " + std::to_string(col) + " " + std::to_string(val);
            try {
                s.send(c.encode({cmd}));
            } catch (...) {
                std::cout << "[ERROR] Connection lost." << std::endl;
                break; 
            }
        } else {
            std::cout << "Usage: Row Col Val (e.g., 1 3 5)" << std::endl;
        }
    }
    
    return 0;
}
