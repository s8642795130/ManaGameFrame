#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <sys/time.h>

#include "PollNet.h"

void Usge(const char* proc) {
	printf("%s[local_ip] [local_port]\n", proc);
}

int array[500];

int PollNet::InitSocket(std::string& ip, int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket error");
		exit(1);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip.c_str());

	bind(sock, (struct sockaddr*)&local, sizeof(local));
	listen(sock, 10);
	return sock;
}

void PollNet::StartPoll(std::string& ip, int port)
{
	int sock = InitSocket(ip, port);

	struct pollfd peerfd[1024];
	peerfd[0].fd = sock;
	peerfd[0].events = POLLIN;

	int nfds = 1;
	int ret;
	int maxsize = sizeof(peerfd) / sizeof(peerfd[0]);
	int timeout = -1;
	for (int i = 0; i < maxsize; i++)
	{
		peerfd[i].fd = -1;
	}

	while (true)
	{
		switch (ret = poll(peerfd, nfds, timeout))
		{
		case 0:
			printf("timeout....\n");
			break;
		case -1:
			printf("poll error\n");
			break;
		default:
		{
			if (peerfd[0].revents & POLLIN)
			{
				struct sockaddr_in client;
				socklen_t len = sizeof(client);
				int new_sock = accept(sock, (struct sockaddr*)&client, &len);
				printf("accpet finish: %d\n", new_sock);
				printf("get a new client#\n");
				int j = 1;
				for (; j < maxsize; j++)
				{
					if (peerfd[j].fd < 0)
					{
						peerfd[j].fd = new_sock;
						peerfd[j].events = POLLIN;
						if (j + 1 > nfds)
						{
							nfds = j + 1;
						}
						break;
					}
				}
			}

			//
			for (int i = 1; i < nfds; i++)
			{
				if (peerfd[i].revents & POLLIN)
				{
					printf("read readly:\n");
					char buf[1024];
					ssize_t s = read(peerfd[i].fd, buf, sizeof(buf) - 1);
					if (s > 0) {
						buf[s] = 0;
						printf("client say# %s\n", buf);
						fflush(stdout);
						peerfd[i].events = POLLOUT;
					}
					else
					{
						close(peerfd[i].fd);
						peerfd[i].fd = -1;
					}
				}

				if (peerfd[i].revents & POLLOUT)
				{
					char* msg = "duanzhihong";
					write(peerfd[i].fd, msg, strlen(msg));
					peerfd[i].events = POLLIN;
				}
			}
		}
		break;
		}
	}
}