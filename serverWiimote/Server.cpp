#include "Server.h"

Server::Server(const int & port, Peripherique * peripherique) : m_port(port), m_peripherique(peripherique) {
	Connect();
}
void Server::Connect() {

	WSAStartup(MAKEWORD(2, 0), &WSAData);
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	m_sin.sin_addr.s_addr = INADDR_ANY;
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(m_port);
	bind(m_sock, (SOCKADDR *)&m_sin, sizeof(m_sin));
	listen(m_sock, 0);
	int sinsize = sizeof(m_csin);
	if ((m_csock = accept(m_sock, (SOCKADDR *)&m_csin, &sinsize)) != INVALID_SOCKET)
	{
		//	send(csock, "Hello world!\r\n", 14, 0);
		m_peripherique->Connected(m_csock);

		//q"aaqclosesocket(csock);
	}

}

