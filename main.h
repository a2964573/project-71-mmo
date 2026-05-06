#ifndef _MAIN_H_
#define _MAIN_H_

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <syncstream>
#include <queue>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define PORT         8080
#define MAX_EVENTS   10
#define WORKER_COUNT 3

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

class GameServer
{
	private:
        int port;
        int worker_count;
        int server_fd;
        int epoll_fd;

        std::queue<Job> job_que;
        std::mutex que_mtx;
        std::condition_variable cv;

        std::unordered_map<int, std::vector<char>> session_buffers;
        std::unordered_set<int> active_clients;
        std::mutex client_mtx;

		int broadcast(char* data, int size, int sender_fd);
		void handleMovePacket(int client_socket, char* data, int size);
		int workerLogic(int worker_id);
		int procedure(int epoll_fd, int client_socket);

	public:
        GameServer(int _port, int _worker_count) : port(_port), worker_count(_worker_count) {}
        ~GameServer();
		bool Start();
};

#endif // _MAIN_H_

