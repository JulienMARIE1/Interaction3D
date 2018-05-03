#include "Peripherique.h"
#include "wiiuse.h"
#include <iostream>
class WiiMote : public Peripherique
{

	private:
		struct wiimote_t* m_wm;
	public:
		WiiMote();
		void Event(SOCKET csock);
		virtual int Connected(SOCKET csock);
		~WiiMote();


};
