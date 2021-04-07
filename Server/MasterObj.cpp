#include <cstring>
#include <iostream>

#include "MasterObj.h"

void MasterObj::ConnectMaster()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // sockaddr_in
    std::memset(&m_client_sin, 0, sizeof(m_client_sin));
    m_client_sin.sin_family = AF_INET;
    m_client_sin.sin_port = htons(3000); //
    m_client_sin.sin_addr.s_addr = inet_addr("192.168.1.105"); //

    //
    if (connect(sock, (struct sockaddr*)&m_client_sin, sizeof(m_client_sin)) < 0)
    {
        std::cout << "connect" << std::endl;
    }

    // char sendbuf[1024 * 4];
    /*
    char recvbuf[BUFFER_SIZE];

    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        // buf
        send(sock_cli, sendbuf, strlen(sendbuf), 0); //
        if (strcmp(sendbuf, "exit\n") == 0)
            break;
        recv(sock_cli, recvbuf, sizeof(recvbuf), 0); //
        fputs(recvbuf, stdout);

        memset(sendbuf, 0, sizeof(sendbuf)); //
        memset(recvbuf, 0, sizeof(recvbuf));
    }
    close(sock_cli);
    */
    
    // test
    SendData(4201);
}