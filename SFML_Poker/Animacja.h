#pragma once
#include <SFML/Graphics.hpp>


//using namespace sf;

class Animacja
{
public:
	float Frame, speed;
	sf::Sprite sprite;
	std::vector<sf::IntRect> frames;

	Animacja() {}

	Animacja(sf::Texture &t, int x, int y, int w, int h, int count, int ileWPoziomie, float Speed)
	{
		// ileWPoziomie = 0 -> pobierz tylko w jednym poziomie
		Frame = 0;
		speed = Speed;
		int ktoryWX = 0;
		int ktoryWY = 0;

		for (int i = 0; i < count; i++) {
			frames.push_back(sf::IntRect(x + ktoryWX*w, ktoryWY+y, w, h));
			ktoryWX++;
			if (ktoryWX == ileWPoziomie && ileWPoziomie != 0)
			{
				ktoryWX = 0;
				ktoryWY += h;
			}
		}

		sprite.setTexture(t);
		sprite.setOrigin(w / 2, h / 2);
		sprite.setTextureRect(frames[0]);
	}

	sf::Sprite getSprite() { return sprite; }
	void pomniejszSprite() {
		sprite.scale(0.5, 0.5);
	}
	void update()
	{
		Frame += speed;
		int n = frames.size();
		if (Frame >= n) Frame -= n;
		if (n>0) sprite.setTextureRect(frames[int(Frame)]);
	}

	bool isEnd()
	{
		return Frame + speed >= frames.size();
	}

};