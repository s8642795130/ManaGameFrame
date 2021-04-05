#include <iostream>

#include <cstring>

#include "ConfigFile.h"
#include "Application.h"
#include "ServerNet.h"
#include "Client.h"

void SendData(int socket, const int& value)
{
    std::array<char, sizeof(value) + 4> buffer;
    int len = sizeof(value);
    std::memcpy(buffer.data(), &len, sizeof(len));
    std::memcpy(buffer.data() + 4, &value, sizeof(value));
    send(socket, buffer.data(), buffer.size(), 0);
}

int ConnectMaster()
{
    int sock_cli = socket(AF_INET, SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(3000);  // 服务器端口
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.105"); // 服务器ip，inet_addr用于IPv4的IP转换（十进制转换为二进制）
    //127.0.0.1是本地预留地址
    //连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        std::cout << "connect" << std::endl;
    }

    
    char sendbuf[1024 * 4];
    /*
    char recvbuf[BUFFER_SIZE];

    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        // 每次读取一行，读取的数据保存在buf指向的字符数组中，成功，则返回第一个参数buf
        send(sock_cli, sendbuf, strlen(sendbuf), 0); ///发送
        if (strcmp(sendbuf, "exit\n") == 0)
            break;
        recv(sock_cli, recvbuf, sizeof(recvbuf), 0); ///接收
        fputs(recvbuf, stdout);

        memset(sendbuf, 0, sizeof(sendbuf));//接受或者发送完毕后把数组中的数据全部清空（置0）
        memset(recvbuf, 0, sizeof(recvbuf));
    }
    close(sock_cli);
    */

    SendData(sock_cli, 4201);

    return sock_cli;
}

int main(int argc, char** argv)
{
	std::cout << "test !!!" << std::endl;

	// application
	std::shared_ptr<Application> app{ std::make_shared<Application>() };
	app->StartThreadPool();

	// Server Control Test
	int master_socket = ConnectMaster();

	ServerNet<Client> server("0.0.0.0", 3010, 30);
    server.TempHandleAccept(master_socket);
	server.EventLoop();

	return 0;
}