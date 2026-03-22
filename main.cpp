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
	std::cout << "Sudoku Server is running on port 1234..." << std::endl;
        server.run([&codec, &gameBoard](Socket client) {
            auto data = client.receive();
            if (data.empty()) return;

            std::string msg(data.begin(), data.end());
            std::vector<std::string> commands = codec.decode(msg);

            if (commands.size() >= 4 && commands[0] == "PUT") {
                try {
                    int r = std::stoi(commands[1]);
                    int c = std::stoi(commands[2]);
                    int v = std::stoi(commands[3]);

                    if (gameBoard.isValid(r, c, v)) {
                        gameBoard.setCell(r, c, v);
                  std::cout << "[" << r << "," << c << "] = " << v << std::endl;
                        
		std::vector<std::string> response = { gameBoard.toString() };
                        client.send(codec.encode(response));
                    } else {
                       client.send(codec.encode({"ERROR", "Invalid move"}));
                    }
                } catch (const std::exception& e) {
                    client.send(codec.encode({"ERROR", "Invalid data format"}));
                }
            } else {
                client.send(codec.encode({"ERROR", "Unknown command"}));
            }
        });
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
