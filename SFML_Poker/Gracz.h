#pragma once
#include "Obiekt.h"
using namespace sf;

class Gracz : public Obiekt
{
private:
	int karty[5];
	bool kartaPodniesiona[5];
	int czyCzteryKartyPodniesione;

public:


	Gracz() { 
		id = "player";
		for (int i = 0; i < 5; i++)	kartaPodniesiona[i] = false;
		czyCzteryKartyPodniesione = 0;
	}
	~Gracz() {};

	bool sprawdzCzyPodniesiona(int ktoraKarta) { return kartaPodniesiona[ktoraKarta]; }
	
	void zmienPodniesiona(int ktoraKarta) {
		if (kartaPodniesiona[ktoraKarta] == false) {
			kartaPodniesiona[ktoraKarta] = true;
			czyCzteryKartyPodniesione++;
			std::cout << czyCzteryKartyPodniesione << std::endl;
		}
		else 
		{
			kartaPodniesiona[ktoraKarta] = false;
			czyCzteryKartyPodniesione--;
			std::cout << czyCzteryKartyPodniesione << std::endl;
		}

	}

	bool czyCzteryKartyWybrane() {
		if (czyCzteryKartyPodniesione == 5)	return true;
		else return false;
	}

	void resetIleKartWybranych() { 
		czyCzteryKartyPodniesione = 0;
		for (int i = 0; i < 5; i++)	kartaPodniesiona[i] = false;
	}


	void resetKarta(int ktoraKarta) {
		czyCzteryKartyPodniesione--;
		kartaPodniesiona[ktoraKarta] = false;
	}

	int getKarta(int ktoraKarta) { return karty[ktoraKarta]; }
	void setKarta(int ktoraKarta, int jakaKarta) { karty[ktoraKarta] = jakaKarta; }
	void update()
	{
	}

};
