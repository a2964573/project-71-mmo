#include "main.h"

int main()
{       
	GameServer my_server(PORT, 3);

	my_server.Start();

    return 0;
}

