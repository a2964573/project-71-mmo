#include "main.h"

#pragma pack(push, 1)
typedef struct {
	unsigned short length;
	unsigned short id;
} PacketHeader;

typedef struct {
	PacketHeader header;
	int x;
	int y;
} MovePacket;
#pragma pack(pop)

typedef struct {
	int client_socket;
	unsigned short packet_id;
	std::vector<char> packet_data;
} Job;

std::queue<Job> job_que;
std::mutex que_mtx;
std::condition_variable cv;

std::unordered_map<int, std::vector<char>> session_buffers;

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
        
		if(crnt_job.packet_id == 1) {
			MovePacket* mov_pkt = reinterpret_cast<MovePacket*>(crnt_job.packet_data.data());

			std::cout << "  [Worker " << worker_id << "] complate! FD: " << crnt_job.client_socket
				<< " X: " << mov_pkt->x << ", "
				<< " Y: " << mov_pkt->y << "\n" << std::flush;
		
			send(crnt_job.client_socket, crnt_job.packet_data.data(), crnt_job.packet_data.size(), 0);
		}
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

	auto& user_buffer = session_buffers[client_socket];
	user_buffer.insert(user_buffer.end(), buffer, (buffer + read_bytes));

	int offset = 0;
	while((user_buffer.size() - offset) >= sizeof(PacketHeader)) {
		PacketHeader* header = reinterpret_cast<PacketHeader*>(user_buffer.data() + offset);

		if((user_buffer.size() - offset) < header->length) {
			break;
		}

		std::vector<char> complete_packet(
			(user_buffer.begin() + offset),
			(user_buffer.begin() + offset + header->length)
		);

		{
			std::lock_guard<std::mutex> lock(que_mtx);
			job_que.push({client_socket, header->id, complete_packet});
		}
		cv.notify_one();

		offset += header->length;
	}

	user_buffer.erase(user_buffer.begin(), (user_buffer.begin() + offset));

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

				session_buffers[client_socket] = std::vector<char>();

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

