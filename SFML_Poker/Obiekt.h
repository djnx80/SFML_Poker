#pragma once

#include "Animacja.h"
using namespace sf;


class Obiekt
{
protected:
	float x, y;
	std::string id;
	bool czyJestNaPlanszy;

public:
	Animacja anim;
	sf::Sprite getSprite() {
		return anim.getSprite();
	}

	bool getLife() { return czyJestNaPlanszy; }
	Animacja getAnim() { return anim; }
	std::string getName() { return id; }

	void setX(float nowaWartosc) { x = nowaWartosc; }
	void setY(float nowaWartosc) { y = nowaWartosc; }
	void setAnim(Animacja nowaWartosc) { anim = nowaWartosc; }
	void setLife(bool nowaWartosc) { czyJestNaPlanszy = nowaWartosc; }

	Obiekt()
	{
		czyJestNaPlanszy = true;
	}

	void settings(Animacja &a, int X, int Y)
	{
		anim = a;
		x = X; y = Y;
	}

	virtual void update() {};

	void draw(RenderWindow &app)
	{
		anim.sprite.setPosition(x, y);
		app.draw(anim.sprite);
	}

	virtual ~Obiekt() {};
};

