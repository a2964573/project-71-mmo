#include "main.h"

#define PORT 8080

int proccedure(int client_socket)
{
	int  read_bytes   = 0;
	char buffer[1024] = {0,};
	const char* response = "Welcome to the Server!";

	while( true ) {
		memset(buffer, 0x00, sizeof(buffer));
		read_bytes = read(client_socket, buffer, sizeof(buffer));
		if( read_bytes <= 0 ) {
			std::cout << "  read len zero. " << std::endl;
			break;
		}
		std::cout << "  $ [" << std::this_thread::get_id() << "][" << client_socket << "]: " << buffer << std::endl;

		send(client_socket, response, strlen(response), 0);
	}

	close(client_socket);
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

	if( listen(server_fd, 3) < 0 ) {
		std::cerr << "listen failed." << std::endl;
		return -1;
	}
	std::cout << "--- server start. port[" << PORT << "] wait..." << std::endl;

	int addrlen;
	int client_socket;

	while( true ) {
		addrlen = sizeof(address);
		client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		if( client_socket < 0 ) {
			std::cerr << "accept failed." << std::endl;
			break;
		}
		std::cout << "  client connect success!" << std::endl;

		std::thread t(proccedure, client_socket);
		t.detach();
	}

	close(server_fd);
	std::cout << "--- server end. ---" << std::endl;
	return 0;
}

