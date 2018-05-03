#include <winsock2.h>
#include "Peripherique.h"
#pragma comment(lib, "ws2_32.lib")

class Server {
	private : 
		SOCKET		  m_csock;
		int			  m_port;
		WSADATA		  WSAData;
		SOCKET		  m_sock;
		SOCKADDR_IN   m_sin;
		SOCKADDR_IN   m_csin;
		Peripherique* m_peripherique;
	public :
		Server(const int & port, Peripherique * peripherique);
		void Connect();
};