#include<SDL.h>
#include<SDL_image.h>
#include "spritesEnum.cpp"
#include <iostream>

class Personagem {
public:

	// direction --> controla se o personagem está olhando para a direita ou para a esquerda.

	int speed = 5;
	int direction; // (1) --> Right | (-1) --> Left
	bool isWalking;
	SDL_Rect rect = {};
	SDL_Rect spriteClipRect;
	SDL_Texture* texture;

	Personagem(int x, int y, SDL_Texture* initialTexture) {

		this->rect.x = x;
		this->rect.y = y;
		this->rect.w = 64;
		this->rect.h = 64;

		this->spriteClipRect;
		this->texture = initialTexture;
		this->isWalking = false;
		this->direction = 1;
	}

	void setSprite(spritesEnum sprite) {

		//std::cout << "Sprite pego: " << sprite << "\n";

		switch (sprite)
		{
		case spritesEnum::MarioLeft:

			this->spriteClipRect = { 24, 48, 16, 24 };
			break;
		case spritesEnum::MarioRight:

			this->spriteClipRect = { 964, 48, 16, 24 };
			break;
		default:
			this->spriteClipRect = { 24, 48, 16, 24 };
			break;
		}
	}

	void setAnimation() {

		Uint32 ticks = SDL_GetTicks();
		Uint32 sprite = (ticks / 100) % 3;

		std::cout << "SPRITE VAR: " << sprite << "\n";
		SDL_Rect walking_left_anim = { 180 + (int) sprite * 52, 48, 16, 24 };
		SDL_Rect walking_right_anim = { 808 - (int) sprite * 52, 48, 16, 24 };

		if (direction == 1) { // Right
			this->spriteClipRect = walking_right_anim;
		}
		else if (direction == -1) {
			this->spriteClipRect = walking_left_anim;
		}

		if (this->isWalking == false) {
			if (direction == 1) {  // sprite right;
				this->spriteClipRect = { 964, 48, 16, 24 };
			}
			else if (direction == -1) {
				this->spriteClipRect = { 24, 48, 16, 24 };
			}
		}
	}
};