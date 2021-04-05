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

    ///����sockaddr_in
    struct sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(3000);  // �������˿�
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.105"); // ������ip��inet_addr����IPv4��IPת����ʮ����ת��Ϊ�����ƣ�
    //127.0.0.1�Ǳ���Ԥ����ַ
    //���ӷ��������ɹ�����0�����󷵻�-1
    if (connect(sock_cli, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        std::cout << "connect" << std::endl;
    }

    
    char sendbuf[1024 * 4];
    /*
    char recvbuf[BUFFER_SIZE];

    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        // ÿ�ζ�ȡһ�У���ȡ�����ݱ�����bufָ����ַ������У��ɹ����򷵻ص�һ������buf
        send(sock_cli, sendbuf, strlen(sendbuf), 0); ///����
        if (strcmp(sendbuf, "exit\n") == 0)
            break;
        recv(sock_cli, recvbuf, sizeof(recvbuf), 0); ///����
        fputs(recvbuf, stdout);

        memset(sendbuf, 0, sizeof(sendbuf));//���ܻ��߷�����Ϻ�������е�����ȫ����գ���0��
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