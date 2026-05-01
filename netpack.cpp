#include "main.h"


#pragma pack(push, 1)
struct PacketHeader {
	unsigned short length;
	unsigned short id;
};

struct MovePacket {
	PacketHeader header;
	int x;
	int y;
};
#pragma pack(pop)


int main()
{
	std::cout << "------- main -------" << std::endl;

	char recv_buffer[1024] = {0,};

	// recv1
	{
		MovePacket rcv_pkt1;
		rcv_pkt1.header.length = sizeof(MovePacket);
		rcv_pkt1.header.id     = 1;
		rcv_pkt1.x = 10;
		rcv_pkt1.y = 20;
		memcpy(recv_buffer, &rcv_pkt1, sizeof(MovePacket));
	}

	// recv2
	{
		MovePacket rcv_pkt2;
		rcv_pkt2.header.length = sizeof(MovePacket);
		rcv_pkt2.header.id     = 1;
		rcv_pkt2.x = 15;
		rcv_pkt2.y = 25;
		memcpy((recv_buffer + sizeof(MovePacket)), &rcv_pkt2, sizeof(MovePacket));
	}

	int recv_len = sizeof(MovePacket) * 2;
	std::cout << "  recv_len[" << recv_len << "]" << std::endl;

	int offset = 0;

	while((recv_len - offset) >= (int)sizeof(PacketHeader)) {
		PacketHeader* header = reinterpret_cast<PacketHeader*>(recv_buffer + offset);

		if((recv_len - offset) < header->length) {
			break;
		}

		if(header->id == 1) {
			MovePacket* mov_pkt = reinterpret_cast<MovePacket*>(recv_buffer + offset);
			std::cout << "  - recv -" << std::endl;
			std::cout << "    move_packet x[" << mov_pkt->x << "]" << std::endl;
			std::cout << "    move_packet y[" << mov_pkt->y << "]" << std::endl;
		}

		offset += header->length;
	}

	std::cout << "------- endl -------" << std::endl;
	return 0;
}

