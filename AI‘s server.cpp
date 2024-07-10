#include <iostream>
#include <thread>
#include <vector>
#include <Winsock2.h>

#pragma comment(lib, "ws2_32.lib")

std::mutex mtx;

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8888); // 设置端口号为8888

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 30) == SOCKET_ERROR) {
        std::cerr << "listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::vector<std::thread> clients;

    while (true) {
        SOCKET clientSocket = INVALID_SOCKET;
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);

        clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed with error: " << WSAGetLastError() << std::endl;
            continue;
        }

        // 创建新线程来处理客户端连接
        clients.emplace_back([clientSocket]() {
            char welcomeMsg[] = "连接成功";
            send(clientSocket, welcomeMsg, strlen(welcomeMsg), 0);

            char buffer[1024] = {0};
            while (true) {
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesReceived <= 0) {
                    break;
                }

                // 假设已经调用了AI处理
                char answer[] = "答案";
                send(clientSocket, answer, sizeof(answer), 0);
                memset(buffer, 0, sizeof(buffer));
            }

            closesocket(clientSocket);
        });
    }

    // 关闭所有线程（在实际应用中，您需要更优雅地处理线程的关闭）
    for (auto& client : clients) {
        if (client.joinable()) {
            client.join();
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}