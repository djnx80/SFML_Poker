#include "stdafx.h"
#include "Plansza.h"
#include <iostream>
#include "SFML/Audio.hpp"

using namespace std;

Plansza::Plansza()
{
	ileWPuli = stawka = stawkaGracz = stawkaPrzeciwnik = 0;
	kasaGracz = kasaPrzeciwnik = 1000;

	gameOver.loadFromFile("images/GameOver1.png");
	pause.loadFromFile("images/Pauza.png");
	menu.loadFromFile("images/menu.png");
	textTlo.loadFromFile("images/background.png");
	tlo.setTexture(textTlo);
	textPanel.loadFromFile("images/panel.png");
	panel.setTexture(textPanel);
	panel.setPosition(260, 640);

	textChipsy.loadFromFile("images/chips.png");
	chipsy[0].setTexture(textChipsy);
	chipsy[0].setTextureRect(sf::IntRect(0, 0, 68, 68));
	chipsy[0].setPosition(450, 500);
	chipsy[1].setTexture(textChipsy);
	chipsy[1].setTextureRect(sf::IntRect(0, 262, 68, 68));
	chipsy[1].setPosition(530, 500);
	chipsy[2].setTexture(textChipsy);
	chipsy[2].setTextureRect(sf::IntRect(0, 372, 68, 68));
	chipsy[2].setPosition(610, 500);
	chipsy[3].setTexture(textChipsy);
	chipsy[3].setTextureRect(sf::IntRect(0, 440, 68, 68));
	chipsy[3].setPosition(690, 500);

}

Plansza::~Plansza()
{
}

void Plansza::AI_podbijaStawke(sf::RenderWindow &app, int ileDodac) {
	if (ileDodac <= kasaPrzeciwnik && ileDodac > 0) {
		stawkaPrzeciwnik += ileDodac;
		kasaPrzeciwnik -= ileDodac;
		ileWPuli = stawkaGracz + stawkaPrzeciwnik;
		sf::String napis = "Przeciwnik podbija stawke o " + std::to_string(ileDodac);
		napisz(app, sf::Vector2f(370, 700), napis);
		app.display();
		czekaj(1500);
	}
}

void Plansza::wyrownajStawki()	{
	// trzeba obliczyc roznice i odjac od kasy
	int roznica = stawkaGracz - stawkaPrzeciwnik;
//	std::cout << "roznica = " << roznica << "\n";
	stawkaGracz = stawkaPrzeciwnik;
//	std::cout << "roznica = " << stawkaGracz<< "\n";
//	std::cout << "roznica = " << stawkaPrzeciwnik << "\n";
	ileWPuli = stawkaGracz + stawkaPrzeciwnik;
	kasaGracz += roznica;
}
void Plansza::ostatnieDodanieDoStawki(int ileDodac) {
	stawkaGracz += ileDodac;
	kasaGracz -= ileDodac;
	int roznica = stawkaGracz - stawkaPrzeciwnik;
	if (stawkaGracz > stawkaPrzeciwnik) {
		stawkaGracz = stawkaPrzeciwnik;
		kasaGracz += roznica;
	}
	grajDzwiek("images/dieShuffle1.wav");
}
bool Plansza::AI_wyrownujeStawke() {
	int ileDodac = stawkaGracz - stawkaPrzeciwnik;
	std::cout << "ile dodac =" << ileDodac << std::endl;
	if (ileDodac <= kasaPrzeciwnik && ileDodac >= 0) {
//		stawkaPrzeciwnik += ileDodac;
		stawkaPrzeciwnik = stawkaGracz;
		kasaPrzeciwnik -= ileDodac;
		ileWPuli = stawkaGracz + stawkaPrzeciwnik;
		return true;
	}

	return false;
}
void Plansza::dodajDoStawki(int ileDodac) {
	if (ileDodac <= kasaGracz) {
		grajDzwiek("images/dieShuffle1.wav");
		stawkaGracz += ileDodac;
		kasaGracz -= ileDodac;
	}
	ileWPuli = stawkaGracz + stawkaPrzeciwnik;
}
void Plansza::noweRozdanie() {
	stawkaGracz = stawkaPrzeciwnik = 10;
	kasaGracz -= 10;
	kasaPrzeciwnik -= 10;
	ileWPuli = stawkaGracz + stawkaPrzeciwnik;
}
void Plansza::rzucKarty(sf::RenderWindow &app) {
	ileWPuli = stawkaGracz + stawkaPrzeciwnik;
	kasaPrzeciwnik += ileWPuli;
	sf::String napis = "Rzuciles karty. Przeciwnik wygrywa " + to_string(ileWPuli);
	napisz(app, sf::Vector2f(370, 700), napis);
	app.display();
//	czekaj(2500);
}
void Plansza::AI_RzucilKarty(sf::RenderWindow &app) {
	ileWPuli = stawkaGracz + stawkaPrzeciwnik;
	kasaGracz += ileWPuli;
	sf::String napis = "Przeciwnik rzucil karty. Wygrales " + to_string(ileWPuli);
	napisz(app, sf::Vector2f(370, 700), napis);
	app.display();
	czekaj(500);
}
void Plansza::rysujPlansze(sf::RenderWindow &app) {
	app.draw(tlo);
	app.draw(panel);
	app.draw(chipsy[0]);
	app.draw(chipsy[1]);
	app.draw(chipsy[2]);
	app.draw(chipsy[3]);
	napisz(app, sf::Vector2f(475, 520), "5");
	napisz(app, sf::Vector2f(555, 520), "10");
	napisz(app, sf::Vector2f(630, 520), "20");
	napisz(app, sf::Vector2f(710, 520), "50");

	sf::String napis;
	napis = "PULA " + to_string(ileWPuli) + "\n     Ty " + to_string(stawkaGracz) + "\n      AI " + to_string(stawkaPrzeciwnik);
	napisz(app, sf::Vector2f(150, 320), napis);
	napis = "Ty " + to_string(kasaGracz) + "\nAi " + to_string(kasaPrzeciwnik);
	napisz(app, sf::Vector2f(900, 320), napis);

	Vector2i mx = Mouse::getPosition(app);
	napis = "x=" + std::to_string(mx.x) + "  y=" + std::to_string(mx.y) + "  ";
	napisz(app, sf::Vector2f(50, 50), napis);

}
void Plansza::pauza(sf::RenderWindow &app) {
	Util util;
	sf::Sprite temp(pause);
	temp.setPosition(250, 280);
	app.draw(temp);
	app.display();
	util.czekaj(200);
	do	{	} while (!sf::Keyboard::isKeyPressed(sf::Keyboard::P));
	util.czekaj(200);
}
void Plansza::koniecGry(sf::RenderWindow &app) {
	std::cout << "game over" << std::endl;
	Util util;
	sf::Sprite temp(gameOver);
	temp.setPosition(325, 250);
	app.draw(temp);
	app.display();
	util.czekaj(1500);
}
bool Plansza::setCzcionka(sf::String fontname, int rozmiar, sf::Color kolor) {
	if (!font.loadFromFile(fontname)) {
		cout << "Nie udalo sie wczytac czcionki" << endl;
		return false;
	}

	// wybranie czcionki
	text.setFont(font);
	text.setCharacterSize(rozmiar); // w pikselach, nie punktach!
	text.setColor(kolor);
	return true;
}
void Plansza::napisz(sf::RenderWindow &okno, sf::Vector2f pozycjaNapisu, sf::String coNapisac) {
	text.setPosition(pozycjaNapisu);
	text.setString(coNapisac);
	okno.draw(text);
}
void Plansza::wygrales(sf::RenderWindow &app) {
	ileWPuli = stawkaGracz + stawkaPrzeciwnik;
	kasaGracz += ileWPuli;
	sf::String napis = "Wygrales " + to_string(ileWPuli);
	napisz(app, sf::Vector2f(370, 700), napis);
	app.display();
	czekaj(2500);
}
void Plansza::przegrales(sf::RenderWindow &app) {
	ileWPuli = stawkaGracz + stawkaPrzeciwnik;
	kasaPrzeciwnik += ileWPuli;
	sf::String napis = "Przegrales " + to_string(ileWPuli);
	napisz(app, sf::Vector2f(370, 700), napis);
	app.display();
	czekaj(2500);
}
void Plansza::remis(sf::RenderWindow &app) {
	ileWPuli = stawkaGracz + stawkaPrzeciwnik;
	kasaPrzeciwnik += stawkaPrzeciwnik;
	kasaGracz += stawkaGracz;
	sf::String napis = "REMIS.  Bank zwraca Tobie " + std::to_string(stawkaGracz) + "  AI "+std::to_string(stawkaPrzeciwnik);
	napisz(app, sf::Vector2f(370, 700), napis);
	app.display();
	czekaj(2500);
}

void Plansza::grajDzwiek(std::string plik) {
	sf::SoundBuffer buffer;
	buffer.loadFromFile(plik);
	sf::Sound sound(buffer);
	sound.play();
	while (sound.getStatus() == sf::Sound::Playing)
	{
	}

}