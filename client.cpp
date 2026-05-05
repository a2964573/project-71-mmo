#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#pragma pack(push, 1)
struct PacketHeader { unsigned short length; unsigned short id; };
struct MovePacket { PacketHeader header; int x; int y; };
#pragma pack(pop)

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8081);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    char send_buf[1024];
    int offset = 0;

    for(int i=1; i<=3; i++) {
        MovePacket pkt;
        pkt.header.length = sizeof(MovePacket);
        pkt.header.id = 1;
        pkt.x = i * 10;
        pkt.y = i * 20;
        memcpy(send_buf + offset, &pkt, sizeof(MovePacket));
        offset += sizeof(MovePacket);
    }
    
    // 3개의 패킷(36바이트)을 한 번의 send로 뭉쳐서 발사!
    send(sock, send_buf, offset, 0); 
    std::cout << "3개의 이동 패킷을 한 번에 발사했습니다." << std::endl;
    
    close(sock);
    return 0;
}

