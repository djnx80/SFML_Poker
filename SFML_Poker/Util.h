#pragma once
#include <SFML/Graphics.hpp>

class Util
{
private:
	sf::Font font;
	sf::Text text;
	sf::Clock zegar;
	float czasCalkowitySekundy;
	struct strefaDane {
		int ktora;
		bool aktywna;
		int x0, y0, x1, y1;
	} strefa[50];

public:
	Util();
	~Util();

	// funkcje odpowiedzialne za czas
	void restartCzas() { czasCalkowitySekundy += zegar.getElapsedTime().asSeconds(); zegar.restart(); }
	sf::Int32 getCzasMilisekundy() { return zegar.getElapsedTime().asMilliseconds(); }
	float getCzasSekundy() { return zegar.getElapsedTime().asSeconds(); }
	float getCzasCalkowity() { return czasCalkowitySekundy; }
	void czekaj(int);

	// funkcje odpowiedzialne za obsluge zone
	void setZone(int, int, int, int, int);
	int checkZones(sf::RenderWindow &app);
};

