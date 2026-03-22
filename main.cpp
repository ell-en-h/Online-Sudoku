#include "network/TcpServer.hpp"
#include "common/SudokuBoard.hpp"
#include "common/codec.h"
#include <iostream>
#include <vector>
#include <string>

using namespace SimpleNet;

int main() {
    try {
        TcpServer server(1234, 4);
        Codec codec;
        SudokuBoard gameBoard;
        gameBoard.initializeRandom(15);
        server.run([&codec, &gameBoard](Socket client) {
            std::vector<std::string> initial = { gameBoard.toString() };
            client.send(codec.encode(initial));
		while (true) {
                auto data = client.receive();
                if (data.empty()) break;
                std::string msg(data.begin(), data.end());
                std::vector<std::string> commands = codec.decode(msg);
                if (commands.size() >= 4 && commands[0] == "PUT") {
                    try {
                        int r = std::stoi(commands[1]);
                        int c = std::stoi(commands[2]);
                        int v = std::stoi(commands[3]);
                        if (gameBoard.isValid(r, c, v)) {
                            gameBoard.setCell(r, c, v);
                            std::cout << "Update: [" << r << "," << c << "] = " << v << std::endl;

                            std::vector<std::string> res = { gameBoard.toString() };
                            client.send(codec.encode(res));
                        } else {
                            std::vector<std::string> err = {"ERROR", "Invalid move"};
                            client.send(codec.encode(err));
                        }
                    } catch (...) {
                        std::vector<std::string> err = {"ERROR", "Invalid format"};
                        client.send(codec.encode(err));
                    }
                } else {
                    std::vector<std::string> err = {"ERROR", "Unknown command"};
                    client.send(codec.encode(err));
                }
            }
        });
    } catch (const std::exception& e) {
        std::cerr << "Critical Error: " << e.what() << std::endl;
    }
    return 0;
}
