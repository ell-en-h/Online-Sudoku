#include "network/TcpServer.hpp"
#include "common/codec.h"
#include <iostream>
#include <vector>
#include <string>

using namespace SimpleNet;

int main() {
    try {
        TcpServer server(1234, 4);
        Codec codec;
        std::cout << "Sudoku Server is running on port 1234..." << std::endl;

        server.run([&codec](Socket client) {
            auto data = client.receive();
            if (data.empty()) return;
            std::string msg(data.begin(), data.end());
            std::vector<std::string> commands = codec.decode(msg);
            for (const auto& s : commands) {
                std::cout << s << std::endl;
            }
            std::vector<std::string> response = {"OK", "Data_Received"};
            client.send(codec.encode(response));
        });
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
