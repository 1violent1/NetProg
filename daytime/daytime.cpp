#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

constexpr const char* SERVER_IP = "172.16.40.1";
constexpr int PORT = 13;
constexpr size_t BUFFER_SIZE = 256;

int main() {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
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

    char trigger = '\n';
    if (sendto(sock_fd, &trigger, sizeof(trigger), 0, 
               reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
        std::cerr << "Ошибка отправки запроса\n";
        close(sock_fd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(serv_addr);
    ssize_t bytes_received = recvfrom(sock_fd, buffer, sizeof(buffer) - 1, 0, 
                                      reinterpret_cast<sockaddr*>(&serv_addr), &addr_len);

    if (bytes_received < 0) {
        std::cerr << "Ошибка получения ответа\n";
        close(sock_fd);
        return 1;
    }

    buffer[bytes_received] = '\0';
    std::cout << "Ответ от daytime сервера:\n" << buffer;

    close(sock_fd);
    return 0;
}
