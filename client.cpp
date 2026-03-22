#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "network/Socket.hpp"
#include "common/codec.h"

using namespace SimpleNet;

void displayBoard(const std::string& boardStr) {
    if (boardStr.length() != 81) return;
    std::cout << "\n    0 1 2   3 4 5   6 7 8\n";
    std::cout << "  +-------+-------+-------+\n";
    for (int i = 0; i < 9; ++i) {
        if (i > 0 && i % 3 == 0) std::cout << "  +-------+-------+-------+\n";
        std::cout << i << " | ";
        for (int j = 0; j < 9; ++j) {
            char cell = boardStr[i * 9 + j];
            std::cout << (cell == '0' ? '.' : cell) << " ";
            if ((j + 1) % 3 == 0) std::cout << "| ";
        }
        std::cout << "\n";
    }
    std::cout << "  +-------+-------+-------+\n";
}

int main() {
    try {
        Socket clientSocket;
        clientSocket.connect("127.0.0.1", 1234);

        Codec codec;
        std::string input;
        auto initialData = clientSocket.receive();
        if (!initialData.empty()) {
            std::string msg(initialData.begin(), initialData.end());
            std::vector<std::string> res = codec.decode(msg);
            if (!res.empty() && res[0].length() == 81) {
                displayBoard(res[0]);
            }
        }
        while (true) {
 	    if (!std::getline(std::cin, input) || input == "exit") break;
            std::stringstream ss(input);
            std::string word;
            std::vector<std::string> command;
            command.push_back("PUT");
            while (ss >> word) {
                command.push_back(word);
            }
            if (command.size() < 4) {
                continue; 
            }
            clientSocket.send(codec.encode(command));
            auto data = clientSocket.receive();
            if (data.empty()) {
                std::cout << "Connection lost." << std::endl;
                break;
            }
            std::string msg(data.begin(), data.end());
            std::vector<std::string> response = codec.decode(msg);
            if (!response.empty()) {
                if (response[0] == "ERROR") {
                    std::cout << ">> SERVER ERROR: " << (response.size() > 1 ? response[1] : "Invalid move") << std::endl;
                } else if (response[0].length() == 81) {
                    displayBoard(response[0]);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
