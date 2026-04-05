# Online-Sudoku

TCP Networking: Архитектура «клиент-сервер» на базе сокетов.

Multithreading: Сервер использует пул потоков (ThreadPool) для обработки множества соединений.

Smart Validation: Система проверяет ход не только на соответствие правилам Судоку, но и на решаемость.

Real-time Update: Синхронное обновление игрового поля у всех подключенных клиентов.

 Структура проекта

  main.cpp — Точка входа сервера, управление игровой логикой.

  client.cpp — Клиентское приложение с интерфейсом отрисовки.

  network/ — Сетевой слой (TcpServer, Socket, ThreadPool).

  common/ — Общие компоненты (Судоку-движок, протокол передачи данных).

# Сборка сервера
g++ main.cpp network/TcpServer.cpp network/Socket.cpp network/ThreadPool.cpp -I. -o server -lpthread

# Сборка клиента
g++ client.cpp network/Socket.cpp -I. -o client -lpthread

Запуск
./server
./client
