#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <list>
#include <iostream>
#include <algorithm>
#include <functional>
#include "Plansza.h"
#include "Obiekt.h"
#include "Animacja.h"
#include "Gracz.h"
#include "Przeciwnik.h"
#include "Karta.h"
#include "Util.h"

void mieszajKarty(int *, Gracz &, Przeciwnik &);
void grajDzwiek(std::string);
int ai(sf::RenderWindow &, Gracz &, Przeciwnik &, Plansza *, Karta *karty, Animacja *, int *, int, sf::String &);
void rysujWszystko(sf::RenderWindow &, std::list<Obiekt*>, Plansza *);
void sprawdzKarty(Gracz &, Przeciwnik &, std::string, std::string &, int &, int &, int &, int *);
void czekaj(int);

using namespace sf;

const int SzerokoscEkranu = 1207;
const int WysokoscEkranu = 831;
const int AI_RzucKarty = -10;
const int AI_WymienKarty = -20;
const int AI_PodbilStawke = -30;
const int WYNIK_WYGRANA = -10;
const int WYNIK_PRZEGRANA = -20;
const int WYNIK_REMIS = -30;


int main()
{
	srand(time(0));
	std::list<Obiekt*> obiekty;


	RenderWindow app(VideoMode(SzerokoscEkranu, WysokoscEkranu), "SFML JNXEngine v1.0");
	app.setFramerateLimit(60);

	Plansza *plansza = new Plansza();
	plansza->setCzcionka("BornAddict.ttf", 24, sf::Color::White);

	Texture t2, tKarty, tKartaTyl;
	t2.loadFromFile("images/przeciwnik.png");
	tKarty.loadFromFile("images/playingCards.png");
	tKartaTyl.loadFromFile("images/playingCardBacks.png");

	Animacja sPrzeciwnik(t2, 170, 0, 79, 110, 3, 0, 0.05);
	Animacja sPrzeciwnikWygrana(t2, 0, 120, 140, 132, 8, 4, 0.05);
	Animacja sPrzeciwnikPrzegrana(t2, 0, 390, 176, 110, 3, 0, 0.02);

	int pozycjeKart[120] = {		// pozycje sprajtów w pliku
		700 ,380,280 ,950,280 ,760,280 ,570,280 ,380,280 ,190,280 ,0,140 ,1710,140 ,1520,140 ,1140,140 ,760,140 ,950,140 ,1330,420 ,1710,420 ,1520,420 ,1330,420 ,1140,420 ,950,420 ,760,420 ,570,420 ,380,420 ,190,280 ,1710,280 ,1330,280 ,1520,420 ,0,140 ,380,140 ,190,140 ,0,0 ,1710,0 ,1520,0 ,1330,0 ,1140,0 ,950,0 ,760,0 ,380,0 ,0,0 ,190,0 ,570,280 ,1140,700 ,190,700 ,0,560 ,1710,560 ,1520,560 ,1330,560 ,1140,560 ,950,560 ,760,560 ,380,560 ,0,560 ,190,560 ,570
	};
	Animacja sKarta[60];		//sprity kart 0-51 - talia, 52 ty³ karty
	for (int nr = 0; nr < 52; nr++) {
		Animacja tempAnim(tKarty, pozycjeKart[nr * 2], pozycjeKart[nr * 2 + 1], 140, 190, 1, 0, 0);
		tempAnim.pomniejszSprite();
		sKarta[nr] = tempAnim;
	}
	Animacja tempAnim(tKartaTyl, 0, 0, 140, 190, 1, 0, 0);		// ty³ karty
	tempAnim.pomniejszSprite();
	sKarta[52] = tempAnim;


	Przeciwnik przeciwnik;
	przeciwnik.settings(sPrzeciwnik, 480, 100);
	obiekty.push_back(&przeciwnik);

	Gracz gracz;

	Karta karty[20];		// karty do wyœwietlenia, 0-4 moje, 5-9 ai,
	int talia[52];			// talia z numerami kart, 0-4 moje, 5-9 ai, 10+ do dobrania 

	Util myUtil;		// strefy do klikania myszk¹
	myUtil.setZone(1, 265, 645, 395, 695);			// rzuæ karty
	myUtil.setZone(2, 405, 645, 620, 695);			// sprawdŸ / pass
	myUtil.setZone(3, 630, 645, 780, 695);			// wymieñ karty
	myUtil.setZone(4, 790, 645, 930, 695);			// nowe rozdanie

	myUtil.setZone(8, 410, 320, 475, 410);		// karty
	myUtil.setZone(9, 490, 320, 560, 410);
	myUtil.setZone(10, 570, 320, 640, 410);
	myUtil.setZone(11, 650, 320, 720, 410);
	myUtil.setZone(12, 730, 320, 800, 410);

	myUtil.setZone(13, 450, 500, 515, 565);		// ¿etony
	myUtil.setZone(14, 530, 500, 595, 565);
	myUtil.setZone(15, 610, 500, 675, 565);
	myUtil.setZone(16, 690, 500, 755, 565);

	int wybranaStrefa;	// która strefa zosta³a wybrana (klikniêta)
	int ktoraRunda = 0;
	// =0 - start gry, rozdanie kart
	// =1 - mozna rzucic karty, podniesc stawke, check (drop, rise, pass)
	// =11 (ai) - ai sprawdza czy: drop/rise/pass 
	// =2 - mozna rzucic karty, wymienic karty,  check, trzeba wyrownac stawke (drop, rise, change, pass)
	// =22 - ai: to co gracz wyzej poza wyrownaniem stawki (gracz wyrownuje i nie moze juz wiecej teraz postawic)
	// =2a teraz nastêpuje wymiana kart
	// =3 - mozna rzucic karty, podniesc stawke, sprawdzic (drop, rise, check)
	// =33 - ai: drop/wyrownaj/check

	int AI_reakcja = 0;
	sf::String wiadomosc = "";

	while (app.isOpen())
	{
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed)		app.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))		app.close();
		if (Keyboard::isKeyPressed(Keyboard::P))			plansza->pauza(app);
		if (Keyboard::isKeyPressed(Keyboard::G))			plansza->koniecGry(app);

		wybranaStrefa = myUtil.checkZones(app);
		if (wybranaStrefa == 1 && ktoraRunda != 5) {	// rzucasz karty
			// ods³oñ karty przeciwnika
			for (int i = 0; i < 5; i++) {
				karty[i + 5].settings(sKarta[przeciwnik.getKarta(i)], 445 + i * 80, 220);
				obiekty.push_back(&karty[i + 5]);
				rysujWszystko(app, obiekty, plansza);
			}
			grajDzwiek("images/you_lose.ogg");
			plansza->rzucKarty(app);
			przeciwnik.settings(sPrzeciwnikWygrana, 480, 100);
			obiekty.push_back(&przeciwnik);
			rysujWszystko(app, obiekty, plansza);
			myUtil.czekaj(200);
			ktoraRunda = 5;
		}
		if (wybranaStrefa == 2 && ktoraRunda == 1) {		// pierwsze sprawdzenie kart
			AI_reakcja = ai(app, gracz, przeciwnik, plansza, karty, sKarta, talia, ktoraRunda, wiadomosc);
			if (AI_reakcja == AI_RzucKarty) {
				// ods³oñ karty przeciwnika
				for (int i = 0; i < 5; i++) {
					karty[i + 5].settings(sKarta[przeciwnik.getKarta(i)], 445 + i * 80, 220);
					obiekty.push_back(&karty[i + 5]);
					rysujWszystko(app, obiekty, plansza);
				}
				przeciwnik.settings(sPrzeciwnikPrzegrana, 480, 100);
				obiekty.push_back(&przeciwnik);
				ktoraRunda = 5;
				grajDzwiek("images/you_win.ogg");
				for (int i = 0; i < 25; i++)
					rysujWszystko(app, obiekty, plansza);
				plansza->AI_RzucilKarty(app);
			}
			else
			{
				ktoraRunda = 2;
			}
		}

//		sprawdz przeciwnika, nastepna runda
		if (wybranaStrefa == 2 && AI_reakcja != AI_PodbilStawke && (ktoraRunda==2 || ktoraRunda==3)) {
			AI_reakcja = ai(app, gracz, przeciwnik, plansza, karty, sKarta, talia, 2, wiadomosc);
			if (AI_reakcja == AI_RzucKarty) {
				// ods³oñ karty przeciwnika
				for (int i = 0; i < 5; i++) {
					karty[i + 5].settings(sKarta[przeciwnik.getKarta(i)], 445 + i * 80, 220);
					obiekty.push_back(&karty[i + 5]);
					rysujWszystko(app, obiekty, plansza);
				}
				przeciwnik.settings(sPrzeciwnikPrzegrana, 480, 100);
				obiekty.push_back(&przeciwnik);
				ktoraRunda = 5;
				grajDzwiek("images/you_win.ogg");
				for (int i = 0; i < 25; i++)
					rysujWszystko(app, obiekty, plansza);
				plansza->AI_RzucilKarty(app);
			}
			else 
			{
				ktoraRunda = 4;
			}
		}
		
		if (wiadomosc.getSize() > 5 && ktoraRunda==2 && AI_reakcja != AI_PodbilStawke) {
			plansza->napisz(app, sf::Vector2f(370, 700), wiadomosc);
			app.display();
			czekaj(2200);
			wiadomosc = "";
		}

		if (wybranaStrefa == 2 && AI_reakcja != AI_PodbilStawke && ktoraRunda == 4) {		// ostatnie sprawdzenie kart

			// ods³oñ karty przeciwnika
			for (int i = 0; i < 5; i++) {
				karty[i+5].settings(sKarta[przeciwnik.getKarta(i)], 445 + i * 80, 220);
				obiekty.push_back(&karty[i + 5]);
				rysujWszystko(app, obiekty, plansza);
			}
			AI_reakcja = ai(app, gracz, przeciwnik, plansza, karty, sKarta, talia, 3, wiadomosc);
			if (AI_reakcja == WYNIK_WYGRANA) {
				grajDzwiek("images/you_win.ogg");
				przeciwnik.settings(sPrzeciwnikPrzegrana, 480, 100);
				obiekty.push_back(&przeciwnik);
				rysujWszystko(app, obiekty, plansza);
				plansza->wygrales(app);
			}
			else if (AI_reakcja == WYNIK_REMIS) {
				plansza->remis(app);
			}
			else if (AI_reakcja == WYNIK_PRZEGRANA) {
				grajDzwiek("images/you_lose.ogg");
				przeciwnik.settings(sPrzeciwnikWygrana, 480, 100);
				obiekty.push_back(&przeciwnik);
				rysujWszystko(app, obiekty, plansza);
				plansza->przegrales(app);
			}

			ktoraRunda = 5;
		}

		if (wybranaStrefa == 3 && ktoraRunda == 2 && AI_reakcja != AI_PodbilStawke) {		// wymien karty
			for (int i = 0; i < 5; i++) {
				if (gracz.sprawdzCzyPodniesiona(i) == true) {
					gracz.setKarta(i, talia[10 + i]);
					karty[i].settings(sKarta[gracz.getKarta(i)], 445 + i * 80, 364);
					gracz.resetKarta(i);
				}
			}
			ktoraRunda = 3;
		}
		if ((wybranaStrefa == 4 && ktoraRunda == 5) || ktoraRunda==0) {		// nowe rozdanie
			system("cls");
			przeciwnik.settings(sPrzeciwnik, 480, 100);
			obiekty.push_back(&przeciwnik);
			mieszajKarty(talia, gracz, przeciwnik);
			for (int i = 0; i < 5; i++) {
				karty[i].settings(sKarta[gracz.getKarta(i)], 445 + i * 80, 364);
//				karty[i+5].settings(sKarta[przeciwnik.getKarta(i)], 445 + i * 80, 220);
				karty[i + 5].settings(sKarta[52], 445 + i * 80, 220);
				obiekty.push_back(&karty[i]);
				obiekty.push_back(&karty[i+5]);
			}
			plansza->noweRozdanie();
			ktoraRunda = 1;
		}
		if (wybranaStrefa >= 8 && wybranaStrefa <= 12 && ktoraRunda == 2) {
				gracz.zmienPodniesiona(wybranaStrefa - 8);
				if (gracz.czyCzteryKartyWybrane() == true) {
					gracz.zmienPodniesiona(wybranaStrefa - 8);
				}
				else
				{
					grajDzwiek("images/cardSlide1.wav");
					if (gracz.sprawdzCzyPodniesiona(wybranaStrefa - 8) == true) {
						karty[wybranaStrefa - 8].setY(350);
					}
					else if (gracz.sprawdzCzyPodniesiona(wybranaStrefa - 8) == false)
					{
						karty[wybranaStrefa - 8].setY(364);
					}
				}
		}
		if (wybranaStrefa == 13 && (ktoraRunda == 2 || ktoraRunda == 1 || ktoraRunda == 3))	plansza->dodajDoStawki(5);
		if (wybranaStrefa == 14 && (ktoraRunda == 2 || ktoraRunda == 1 || ktoraRunda == 3))	plansza->dodajDoStawki(10);
		if (wybranaStrefa == 15 && (ktoraRunda == 2 || ktoraRunda == 1 || ktoraRunda == 3))	plansza->dodajDoStawki(20);
		if (wybranaStrefa == 16 && (ktoraRunda == 2 || ktoraRunda == 1 || ktoraRunda == 3))	plansza->dodajDoStawki(50);
		if (wybranaStrefa == 13 && ktoraRunda == 4)	plansza->ostatnieDodanieDoStawki(5);
		if (wybranaStrefa == 14 && ktoraRunda == 4)	plansza->ostatnieDodanieDoStawki(10);
		if (wybranaStrefa == 15 && ktoraRunda == 4)	plansza->ostatnieDodanieDoStawki(20);
		if (wybranaStrefa == 16 && ktoraRunda == 4)	plansza->ostatnieDodanieDoStawki(50);
		if (AI_reakcja == AI_PodbilStawke) {
			if (plansza->getStawkaAI() == plansza->getStawkaGracz()) {
				AI_reakcja = 0;
			}
			if (plansza->getStawkaAI() < plansza->getStawkaGracz()) {
				AI_reakcja = 0;
				plansza->wyrownajStawki();
			}
		}
		rysujWszystko(app, obiekty, plansza);
		myUtil.czekaj(40);
	}
	return 0;
}
void rysujWszystko(sf::RenderWindow &app, std::list<Obiekt*> obiekty, Plansza *plansza) {
	for (auto i = obiekty.begin(); i != obiekty.end();)
	{
		Obiekt *e = *i;
		e->update();
		e->anim.update();
		if (e->getLife() == false) 		i = obiekty.erase(i);
		else	i++;

	}

	// rysuj
	app.clear();
	plansza->rysujPlansze(app);
	for (auto i : obiekty)
		i->draw(app);
	app.display();
}
int ai(sf::RenderWindow &app, Gracz &gracz, Przeciwnik &przeciwnik, Plansza *plansza, Karta *karty, Animacja *sKarta, int *talia, int runda, sf::String &wiadomosc) {
	int kartyDoWymiany[5]; // które karty wymieniæ = 0
	std::string coMasz = "nic";			// tutaj: para, dwie pary, trojka, trojka i para
	std::string czyStrit = "nic";		// tutaj: strit, prawie strit (4 karty takie same)
	int figura1, figura2;		// numery figur ktore masz, im wyzszy numer tym wyzsza figura
	figura1 = figura2 = -1;

	int MOC_KART = 0;
	int MOC_FIGUR = 0;
//	wiadomosc = "nic";

	for (int i = 0; i < 5; i++)	kartyDoWymiany[i] = -1;		// wyczyœæ (ai) karty do wymiany
	
	// sprawdz jakie karty ma przeciwnik
	sprawdzKarty(gracz, przeciwnik, "AI", coMasz, figura1, figura2, MOC_KART, kartyDoWymiany);

	// teraz juz wiadomo co ma komputer, wiêc czas na reakcje ai
	std::cout << coMasz << std::endl;

	int ileZagrac = ((rand() % 9) * 5) * MOC_KART;
	if (MOC_KART == 0)
		ileZagrac = (rand() % 7) * 5;

	// RUNDA 1
	if (runda == 1) {
		int tempk = 0;

		std::cout << "RUNDA 1" << std::endl;
		if (coMasz == "nic" && (rand()%3)==1)
			return AI_RzucKarty;	// nic nie ma, wiec rzucam karty
		if (coMasz == "nic" && plansza->getStawkaGracz() > 50 && (rand() % 2) == 1)
			return AI_RzucKarty;	// nic nie ma, a zawodnik sporo postawil

		if (plansza->getStawkaGracz() > 10) {
			// gracz coœ wrzuci³ na stó³ wiêc trzeba wyrównaæ
			if (plansza->AI_wyrownujeStawke() == false) {
				// za ma³o geldów
	//			std::cout << "AI NIE wyrownuje stawki i rzuca kartami" << std::endl;
				return AI_RzucKarty;
			}
		}

		if (ileZagrac > 0)	
		{

			std::cout << "AI podbija stawke o " << ileZagrac << std::endl;
			plansza->AI_podbijaStawke(app, ileZagrac);
			for (int i = 0; i < 5; i++) {
				if (kartyDoWymiany[i] == -1) {
					tempk++;
//					std::cout << "wymieniam karty" << std::endl;
					przeciwnik.setKarta(i, talia[15 + i]);
		//			karty[i + 5].settings(sKarta[przeciwnik.getKarta(i)], 445 + i * 80, 220);
				}
			}
			if (tempk == 5) tempk = 4;
			wiadomosc = "Komputer wymienia " + std::to_string(tempk) + " karty";
			return AI_PodbilStawke;
		}

		// wymieñ ju¿ teraz karty, na nastêpn¹ turê.
		for (int i = 0; i < 5; i++) {
			if (kartyDoWymiany[i] == -1) {
				tempk++;
				przeciwnik.setKarta(i, talia[15 + i]);
	//			karty[i+5].settings(sKarta[przeciwnik.getKarta(i)], 445 + i * 80, 220);
			}
		}
		// ma³e oszustwo, komputer moze wymienic 5 kart, ale gracz siê o tym nie dowie :)
		if (tempk == 5) tempk = 4;
		wiadomosc = "Komputer wymienia "+std::to_string(tempk)+" karty";

	}

	if (runda == 2) {
		std::cout << "drugie sprawdzenie kart" << std::endl;
		// nic nie ma = rzuc karty
		if (coMasz == "nic" && (rand() % 2) == 1)
			return AI_RzucKarty;	// nic nie ma, wiec rzucam karty

		// wyrownaj stawke
		if (plansza->AI_wyrownujeStawke() == false) {
			// za ma³o geldów
			std::cout << "runda 2 AI NIE wyrownuje stawki i rzuca kartami" << std::endl;
			return AI_RzucKarty;
		}

		// w zaleznosc od kart podbij stawke
		std::cout << "ile zagrac =" << ileZagrac << std::endl;
		if (ileZagrac > 0)
		{
			std::cout << "runda2 AI podbija stawke o " << ileZagrac << std::endl;
			plansza->AI_podbijaStawke(app, ileZagrac);
			return AI_PodbilStawke;
		}

	}

	if (runda == 3) {
		std::string mojeKarty = "nic";
		std::string aiKarty = "nic";
		int mfig1, mfig2, aifig1, aifig2;
		int aiMoc, tyMoc;

//		for (int i = 0; i < 5; i++) {
			std::cout << "ostateczne sprawdzenie kart" << std::endl;
			std::cout << "odslaniam karty" << std::endl;
//			karty[i + 5].settings(sKarta[przeciwnik.getKarta(i)], 445 + i * 80, 220);
//		}
		sprawdzKarty(gracz, przeciwnik, "AI", coMasz, figura1, figura2, MOC_KART, kartyDoWymiany);
		aiKarty = coMasz;
		aiMoc = MOC_KART;
		aifig1 = figura1;
		aifig2 = figura2;
		sprawdzKarty(gracz, przeciwnik, "TY", coMasz, figura1, figura2, MOC_KART, kartyDoWymiany);
		mojeKarty = coMasz;
		tyMoc = MOC_KART;
		mfig1 = figura1;
		mfig2 = figura2;
		std::cout << "Wyniki to:" << std::endl;
		std::cout << "AI:" << aiKarty << " moc="<<aiMoc<<"  figury:"<<aifig1<<"  "<<aifig2<< std::endl;
		std::cout << "Ty:" << mojeKarty << " moc=" << tyMoc << "  figury:" << mfig1 << "  " << mfig2 << std::endl;

		if (aiMoc > tyMoc) {
			std::cout << "KOMPUTER WYGRAL\n";
			return WYNIK_PRZEGRANA;
		} else if (aiMoc == tyMoc)	{
			if (aiMoc == 3) {
				// dwie pary, wiec dwie figury do sprawdzenia
				aifig1 = aifig1 + aifig2;
				mfig1 = mfig1 + mfig2;
			}
			if (aifig1 > mfig1) {
				std::cout << "KOMPUTER WYGRAL\n";
				return WYNIK_PRZEGRANA;
			} else if (aifig1 == mfig1)	{
				std::cout << "REMIS\n";
				return WYNIK_REMIS;
			} else if (aifig1 < mfig1) {
				std::cout << "WYGRALES FARCIARZU\n";
				return WYNIK_WYGRANA;
			}

		} else if (aiMoc < tyMoc)	{
			std::cout << "WYGRALES FARCIARZU\n";
			return WYNIK_WYGRANA;
		}
	}

	return 0;		// 0 - brak reakcji
}
void sprawdzKarty(Gracz &gracz, Przeciwnik &przeciwnik, std::string kto, std::string &coMasz, int &figura1, int &figura2, int &MOC_KART, int *kartyDoWymiany) {
	int delta = 13;		// przesuniêcie w kartach, figurach
	int jakieKarty[13];		// jakie karty w jakich ilosciach wystepuja
	coMasz = "nic";
	figura1 = figura2 = -1;

	// wpisz do tablicy po ile razy wystepuja konkretne karty
	for (int i = 0; i < 13; i++) {
		jakieKarty[i] = 0;
		for (int j = 0; j < 5; j++) {
			if (kto == "AI") {
				if (przeciwnik.getKarta(j) == i || przeciwnik.getKarta(j) == i + delta || przeciwnik.getKarta(j) == i + (delta * 2) || przeciwnik.getKarta(j) == i + (delta * 3)) {
					jakieKarty[i]++;
				}
			}
			else if (kto == "TY")
			{
				if (gracz.getKarta(j) == i || gracz.getKarta(j) == i + delta || gracz.getKarta(j) == i + (delta * 2) || gracz.getKarta(j) == i + (delta * 3)) {
					jakieKarty[i]++;
				}
			}
		}
	}

	int temp[5];		// zmniejsza talie do 12 figura, bez kolorów
	for (int i = 0; i < 5; i++) {
		if (kto == "AI") {
			temp[i] = przeciwnik.getKarta(i);
		}
		else if (kto == "TY")
		{
			temp[i] = gracz.getKarta(i);
		}
		if (temp[i] > 12 && temp[i] < 26)		temp[i] -= delta;
		if (temp[i] >= 26 && temp[i] < 39)	temp[i] -= delta * 2;
		if (temp[i] >= 39)					temp[i] -= delta * 3;
	}


	// i sprawdz czy powtarzaj¹ siê figury (nie dzia³a na strit, trzeba osobno rozpisac)
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (i != j && temp[i] == temp[j])
				kartyDoWymiany[i] = 0;
		}
	}

//	for (int i = 0; i < 5; i++) {
//		std::cout << "karta =" << temp[i] << std::endl;
//		std::cout << i << "=" << kartyDoWymiany[i] << std::endl;
//	}

	// sprawdz czy s¹ pary
	for (int i = 0; i < 13; i++) {
		if (jakieKarty[i] == 2) {
			if (coMasz == "para") {
				coMasz = "dwie pary";
				figura2 = i;
				MOC_KART = 3;
			}
			if (coMasz == "nic") {
				coMasz = "para";
				figura1 = i;
				MOC_KART = 1;
			}
			//			std::cout << "para: " << i << std::endl;
		}
	}
	// czy s¹ trójki
	for (int i = 0; i < 13; i++) {
		if (jakieKarty[i] == 3) {
			//			std::cout << "trojka: " << i << std::endl;
			if (coMasz == "nic") {
				coMasz = "trojka";
				figura1 = i;
				MOC_KART = 5;
			}
			if (coMasz == "para") {
				coMasz = "full";
				figura2 = i;
				MOC_KART = 8;
			}

		}
	}
	// a mo¿e czwóreczka
	for (int i = 0; i < 13; i++) {
		if (jakieKarty[i] == 4) {
			//			std::cout << "czworka: " << i << std::endl;
			coMasz = "czworka";
			figura1 = i;
			MOC_KART = 10;
		}
	}

	// a mo¿e masz strit, lub prawie strit
	std::sort(temp, temp + 5, std::greater < int >());		// posotuj tablice malejaco

															// jezeli karty koncza siê na asie to zwieksz wartosc najmniejszych
	if (temp[0] == 12 && temp[1] == 11 && temp[2] == 10 && temp[3] == 9 && temp[4] == 0)
		temp[4] = 8;
	if (temp[0] == 12 && temp[1] == 11 && temp[2] == 10 && temp[3] == 1 && temp[4] == 0) {
		temp[3] = 9;
		temp[4] = 8;
	}
	if (temp[0] == 12 && temp[1] == 11 && temp[2] == 2 && temp[3] == 1 && temp[4] == 0) {
		temp[2] = 10;
		temp[3] = 9;
		temp[4] = 8;
	}
	if (temp[0] == 12 && temp[1] == 3 && temp[2] == 2 && temp[3] == 1 && temp[4] == 0) {
		temp[1] = 11;
		temp[2] = 10;
		temp[3] = 9;
		temp[4] = 8;
	}

	// sprawdz czy masz strita
	int ileWStrit = 0;
	if (coMasz == "nic") {
		for (int i = 0; i < 4; i++) {
			int t1 = temp[i];
			int t2 = temp[i + 1];
			if (t1 - t2 == 1) ileWStrit++;
		}
		int t1 = temp[4];		// sprawdz ostatnia kartê z pierwsz¹
		int t2 = temp[0];
		if (t1 - t2 == -12) ileWStrit++;		// 2 -> As - dlatego ró¿nica 12
		if (ileWStrit == 4) {
			coMasz = "strit";
			MOC_KART = 6;
			for (int i = 0; i < 5; i++) {
				kartyDoWymiany[i] = 0;
			}
		}
		if (ileWStrit == 3) {
			coMasz = "prawie strit";
			// igonrujemy prawie strita, i wymieniamy losowo 4 karty, ale nie rzucamy ich w pierwszej rundzie
		}

	}


}
void mieszajKarty(int *talia, Gracz &gracz, Przeciwnik &przeciwnik) {
	int temp[52];	// tablica wskazuj¹ca czy liczba by³a ju¿ wylosowana (wtedy =-1)
	for (int i = 0; i < 52; i++)	temp[i] = i;

	// mieszaj tablicê
	int n;
	for (int i = 0; i < 52; i++) {
		bool jestOk = false;
		do
		{	// losuj liczbê dopóki siê nie powtarza
			n = (rand() % 52);
			if (temp[n] != -1) {
				jestOk = true;
				temp[n] = -1;
				talia[i] = n;
			}
		} while (jestOk == false);
	}

//	talia[5] = 10;
//	talia[6] = 11;
//	talia[7] = 12;
//	talia[8] = 13;
//	talia[9] = 14;

	grajDzwiek("images/cardFan1.wav");

	// rozdaj karty
	for (int i = 0; i < 5; i++) {
		gracz.setKarta(i, talia[i]);
		przeciwnik.setKarta(i, talia[i+5]);
	}
	gracz.resetIleKartWybranych();
}
void grajDzwiek(std::string plik) {
	sf::SoundBuffer buffer;
	buffer.loadFromFile(plik);
	sf::Sound sound(buffer);
	sound.play();
	while (sound.getStatus() == sf::Sound::Playing)
	{
	}

}
void czekaj(int ileCzasuWMilisekundach) {
	Clock zegarTemp;
	zegarTemp.restart();
	do {		// odczekaj czas
	} while (zegarTemp.getElapsedTime().asMilliseconds()<ileCzasuWMilisekundach);
}