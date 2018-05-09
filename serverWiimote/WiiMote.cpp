#include "WiiMote.h"

WiiMote::WiiMote() {

}

void WiiMote::Event(SOCKET csock) {
	int aPressed = 0;
	std::string str = "";
	if (IS_PRESSED(m_wm, WIIMOTE_BUTTON_A)) {
		aPressed = 1;
	}
	str += std::to_string(aPressed) + ";" + std::to_string(m_wm->orient.a_roll) + ";" + std::to_string(m_wm->orient.a_pitch) + ";";
	const char * sendValue = str.c_str();
	std::cout << sendValue << std::endl;
	send(csock, sendValue, str.size(), 0);
}



short any_wiimote_connected(wiimote** wm, int wiimotes) {
	int i;
	if (!wm) {
		return 0;
	}
	for (i = 0; i < wiimotes; i++) {
		/*	if (wm[i] && WIIMOTE_IS_CONNECTED(wm[i])) {
		return 1;
		}*/
	}
	return 0;
}

int WiiMote::Connected(SOCKET csock){
	wiimote** wiimotes; //Declaration d'un tableau de pointeurs wiimote
	int found, connected;

	//Initialize an array of wiimote objects.
	//The parameter is the number of wiimotes I want to create.
	wiimotes = wiiuse_init(1);

	//system("CLS");
	std::cout << "Wiimote Basic Test\n\n";

	//On cherche des wiimote
	//En paramètres: le tableau de wiimotes, le nombre MAX à chercher, le timout
	//Retourne le nombre de wiimotes trouvées
	found = wiiuse_find(wiimotes, 1, 5);

	if (!found) {
		std::cout << "No wiimotes found.\n";
		Sleep(5000);
		//system("PAUSE");
		return 0;
	}

	//On connecte les wiimotes trouvées
	//paramètres: Le tableau de wiimotes, le nombre de wiimotes trouvées
	//retourne le nombre de wiimotes connectées
	connected = wiiuse_connect(wiimotes, found);

	if (connected)  std::cout << "Connected to " << connected << " wiimotes (of " << found << " found).\n";
	else {
		std::cout << "Failed to connect to any wiimote.\n";
		Sleep(5000);

		//  system("PAUSE");
		return 0;
	}

	//On allume la LED correspondant au numéro pour chaque wiimote (de 1 à 4)
	wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);


	//Vibration pour les wiimotes 1 et 2 de 0.2 seconde
	//Arguments (wiimote à faire vibrer, 1 = vibration ; 0 = arret de la vibration)
	wiiuse_rumble(wiimotes[0], 1);
	//usleep(200000);
	wiiuse_rumble(wiimotes[0], 0);

	wiiuse_motion_sensing(wiimotes[0], 1); // active accelerometre

	wiiuse_set_aspect_ratio(wiimotes[0], WIIUSE_ASPECT_16_9);

	while (true) {
		if (wiiuse_poll(wiimotes, 1)) {
			/*
			* This happens if something happened on any wiimote.
			* So go through each one and check if anything happened.
			*/

			for (int i = 0; i < 1; ++i) {
				switch (wiimotes[i]->event) {
				case WIIUSE_EVENT:
					/* a generic event occurred */
					m_wm = wiimotes[i];
					Event(csock);
					break;
				default:
					std::cout << "default " << std::endl;
					break;
				}
			}
		}
	}
	//Timout des wiimotes, valeur max pour éviter d'avoir des messages d'erreur
	wiiuse_set_timeout(wiimotes, 1, 0xFF, 0xFF);

	//sleep(10);

	//Disconnect the wiimotes
	wiiuse_cleanup(wiimotes, 1);


	return 0;
}

WiiMote::~WiiMote()
{
}
