#include "main.h"


#define PORT 8080
#define MAX_EVENTS 10
#define WORKER_COUNT 3

struct Job {
	int client_socket;
	std::string data;
};

std::queue<Job> job_que;
std::mutex que_mtx;
std::condition_variable cv;

void worker_logic(int worker_id)
{
	while(true) {
		Job crnt_job;
		{
			std::unique_lock<std::mutex> lock(que_mtx);
			cv.wait(lock, [] { return !job_que.empty(); });

			crnt_job = job_que.front();
			job_que.pop();
		}

		std::cout << "  [Worker " << worker_id << " (T_ID: " << std::this_thread::get_id() 
                  << ")] complate! FD: " << crnt_job.client_socket 
                  << " | Data: " << crnt_job.data << std::flush;

		send(crnt_job.client_socket, crnt_job.data.c_str(), crnt_job.data.length(), 0);
	}

	return;
}

int procedure(int epoll_fd, int client_socket)
{
    int  read_bytes   = 0;
    char buffer[1024] = {0,};

    read_bytes = read(client_socket, buffer, sizeof(buffer));
    if( read_bytes <= 0 ) {
        std::cout << "  read len zero. " << std::endl;
		epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_socket, NULL);
        close(client_socket);
        return -1;
    }
    std::cout << "  $ [" << std::this_thread::get_id() << "][" << client_socket << "]: " << buffer << std::endl;

	{
		std::lock_guard<std::mutex> lock(que_mtx);
		job_que.push({client_socket, std::string(buffer)});
	}
	cv.notify_one();

    return 0;
}

int main()
{
	std::vector<std::thread> workers;

	int i;
	for(i = 0; i< WORKER_COUNT; i++) {
		workers.emplace_back(worker_logic, i + 1);
		workers.back().detach();
	}

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
                procedure(epoll_fd, client_socket);
            }
        }
    }

    close(server_fd);
    std::cout << "--- server end. ---" << std::endl;
    return 0;
}

