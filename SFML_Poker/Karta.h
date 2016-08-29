#pragma once
#include "Obiekt.h"

class Karta : public Obiekt
{
private:
	int x, y;

public:
	Karta() { id = "karta"; }
	~Karta() {};
	void update() {};

};

