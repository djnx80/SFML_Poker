#pragma once
#include "Obiekt.h"

class Przeciwnik : public Obiekt
{
private:
	int karty[5];

public:
	Przeciwnik() { 
		id = "przeciwnik";
	}
	~Przeciwnik() {};
	void update() {};
	int getKarta(int ktoraKarta) { return karty[ktoraKarta]; }
	void setKarta(int ktoraKarta, int jakaKarta) { karty[ktoraKarta] = jakaKarta; }
};

