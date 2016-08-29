#pragma once
#include <SFML/Graphics.hpp>
#include "Util.h"
#include "Obiekt.h"

class Plansza
{
	// zawiera menu, plansze koncowe i pauze
private:
	sf::Texture gameOver, pause, menu, textTlo, textPanel, textChipsy;
	sf::Sprite tlo, panel, chipsy[5];
	sf::Font font;
	sf::Text text;

	int stawkaGracz;
	int stawkaPrzeciwnik;
	int ileWPuli;
	int stawka;
	int kasaGracz;
	int kasaPrzeciwnik;

public:
	Plansza();
	~Plansza();
	void pauza(sf::RenderWindow &);
	void koniecGry(sf::RenderWindow &);

	void wygrales(sf::RenderWindow &);
	void przegrales(sf::RenderWindow &);
	void remis(sf::RenderWindow &);

	void ostatnieDodanieDoStawki(int);
	int getStawkaAI() { return stawkaPrzeciwnik; }
	int getStawkaGracz() { return stawkaGracz; }
	void wyrownajStawki();
	void dodajDoStawki(int);
	void AI_podbijaStawke(sf::RenderWindow &, int);
	bool AI_wyrownujeStawke();
	void AI_RzucilKarty(sf::RenderWindow &);
	void rzucKarty(sf::RenderWindow &);
	void noweRozdanie();
	void rysujPlansze(sf::RenderWindow &);
	// funkcje odpowiedzialne za czcionki i napisy
	bool setCzcionka(sf::String, int, sf::Color);
	void napisz(sf::RenderWindow &, sf::Vector2f, sf::String);
	void grajDzwiek(std::string);
	void czekaj(int ileCzasuWMilisekundach) {
		sf::Clock zegarTemp;
		zegarTemp.restart();
		do {		// odczekaj czas
		} while (zegarTemp.getElapsedTime().asMilliseconds()<ileCzasuWMilisekundach);
	}
};

