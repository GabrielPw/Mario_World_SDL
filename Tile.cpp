#include<iostream>
#include<SDL.h>
#include<SDL_image.h>

class Tile {

public:

	int x, y;
	SDL_Texture* texture;
	SDL_Rect rect;

	Tile(int x, int y, SDL_Texture* texture) {

		this->x = x;
		this->y = y;

		this->rect.x = this->x;
		this->rect.y = this->y;

		this->texture = texture;
	}

	void loadMapFromFile(const char file) {

	}
};