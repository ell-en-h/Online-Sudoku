all: server client

server:
	g++ main.cpp network/TcpServer.cpp network/Socket.cpp network/ThreadPool.cpp -I. -o sudoku_server -lpthread

client:
	g++ client.cpp network/Socket.cpp -I. -o sudoku_client

clean:
	rm -f sudoku_server sudoku_client

