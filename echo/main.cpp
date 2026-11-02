#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

constexpr const char* SERVER_IP = "172.16.40.1";
constexpr int PORT = 7;
constexpr size_t BUFFER_SIZE = 1024;

int main() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Некорректный IP-адрес\n";
        close(sock_fd);
        return 1;
    }

    if (connect(sock_fd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
        std::cerr << "Ошибка подключения к серверу " << SERVER_IP << ":" << PORT << "\n";
        close(sock_fd);
        return 1;
    }

    std::string message = "Hello from C++ Echo Client!\n";

    if (send(sock_fd, message.c_str(), message.size(), 0) < 0) {
        std::cerr << "Ошибка отправки данных\n";
        close(sock_fd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received < 0) {
        std::cerr << "Ошибка чтения ответа\n";
        close(sock_fd);
        return 1;
    }

    buffer[bytes_received] = '\0';
    std::cout << "Ответ от echo сервера:\n" << buffer;

    close(sock_fd);
    return 0;
}
