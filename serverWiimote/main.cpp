
#include <thread>
#include "Server.h"
#include "Peripherique.h"
#include "WiiMote.h"





int main(void)
{
//	std::thread wiiMote(get_wiimote_data);
	//std::thread socketServer(socket_server);
//	wiiMote.join();
	//socketServer.join();
	//socket_server();
	Peripherique * periph = new WiiMote;
	int port = 9999;
	Server *server = new Server(port, periph);

	return 0;
}

