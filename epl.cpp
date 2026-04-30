#include "main.h"

#define PORT 8080
#define MAX_EVENTS 10

int procedure(int client_socket)
{
	int  read_bytes   = 0;
	char buffer[1024] = {0,};

	read_bytes = read(client_socket, buffer, sizeof(buffer));
	if( read_bytes <= 0 ) {
		std::cout << "  read len zero. " << std::endl;
		close(client_socket);
		return -1;
	}
	std::cout << "  $ [" << std::this_thread::get_id() << "][" << client_socket << "]: " << buffer << std::endl;

	send(client_socket, buffer, strlen(buffer), 0);

	return 0;
}

int main()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if( server_fd == 0 ) {
		std::cerr << "create socket failed." << std::endl;
		return -1;
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if( bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0 ) {
		std::cerr << "bind failed." << std::endl;
		return -1;
	}

	if( listen(server_fd, SOMAXCONN) < 0 ) {
		std::cerr << "listen failed." << std::endl;
		return -1;
	}

	int epoll_fd = epoll_create1(0);
	struct epoll_event event;
	struct epoll_event events[MAX_EVENTS];

	event.events = EPOLLIN;
	event.data.fd = server_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

	std::cout << "--- server start. port[" << PORT << "]" << std::endl;

	int i;
	int num_ready;
	int client_socket;
	while( true ) {
		num_ready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

		for(i = 0; i < num_ready; i++) {
			if(events[i].data.fd == server_fd) {
				client_socket = accept(server_fd, NULL, NULL);
				if( client_socket < 0 ) {
					std::cerr << "accept failed." << std::endl;
					break;
				}
				std::cout << "  client connect success!" << std::endl;

				event.events = EPOLLIN;
				event.data.fd = client_socket;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event);
			}
			else {
				client_socket = events[i].data.fd;
				procedure(client_socket);
			}
		}
	}

	close(server_fd);
	std::cout << "--- server end. ---" << std::endl;
	return 0;
}

