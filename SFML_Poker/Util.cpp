#include "stdafx.h"
#include "Util.h"
#include <iostream>

using namespace std;

Util::Util()
{
}


Util::~Util()
{
}

void Util::czekaj(int ileCzasuWMilisekundach) {
	sf::Clock zegarTemp;
	zegarTemp.restart();
	do {		// odczekaj czas
	} while (zegarTemp.getElapsedTime().asMilliseconds()<ileCzasuWMilisekundach);
}

void Util::setZone(int ktoraStrefa, int x0, int y0, int x1, int y1) {
	strefa[ktoraStrefa].x0 = x0;
	strefa[ktoraStrefa].y0 = y0;
	strefa[ktoraStrefa].x1 = x1;
	strefa[ktoraStrefa].y1 = y1;
	strefa[ktoraStrefa].aktywna = true;
}

int Util::checkZones(sf::RenderWindow &app) {
	int ktoraStrefa = -1;
	sf::Vector2f pozycjaMyszki;
	pozycjaMyszki = sf::Vector2f(sf::Mouse::getPosition(app).x, sf::Mouse::getPosition(app).y);

	for (int i = 0; i < 50; i++) {
		if (pozycjaMyszki.x > strefa[i].x0 && pozycjaMyszki.x < strefa[i].x1 && pozycjaMyszki.y > strefa[i].y0 && pozycjaMyszki.y < strefa[i].y1 && (sf::Mouse::isButtonPressed(sf::Mouse::Left) == true)) {
			return i;
		}
		if (pozycjaMyszki.x > strefa[i].x0 && pozycjaMyszki.x < strefa[i].x1 && pozycjaMyszki.y > strefa[i].y0 && pozycjaMyszki.y < strefa[i].y1 && (sf::Mouse::isButtonPressed(sf::Mouse::Right) == true)) {
			return i+100;
		}

	}
	return -1;
}