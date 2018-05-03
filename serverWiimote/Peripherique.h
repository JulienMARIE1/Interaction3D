#ifndef PERIPHERIQUE_H
#define PERIPHERIQUE_H
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <string>

class Peripherique
{
public:
	virtual ~Peripherique();
	virtual int Connected(SOCKET m_csock) = 0;

};

#endif