#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<SDL_image.h>
#include <iostream>
#include <vector>
#include <SDL_mixer.h>

enum class spritesEnum
{
	MarioLeft,
	MarioRight
};

class Personagem {
public:

	// direction --> controla se o personagem está olhando para a direita ou para a esquerda.

	int speed = 8;
	int mass = 2;
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

		//std::cout << "SPRITE VAR: " << sprite << "\n";
		SDL_Rect walking_left_anim = { 180 + sprite * 52, 48, 16, 24 };
		SDL_Rect walking_right_anim = { 808 - sprite * 52, 48, 16, 24 };

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


// Methods
bool initVerify(SDL_Window* window, SDL_Renderer* renderer);
void playerCheckMovement(Personagem *personagem, SDL_Texture* textura1, SDL_Texture* textura2);

int main() {

	int WINDOW_WIDTH = 600; int WINDOW_HEIGHT = 600;
	int GRAVITY = 5; int force;

	int mouseX, mouseY;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Minha Janela", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	
	// Disativa renderização do cursor do mouse.
	SDL_ShowCursor(SDL_DISABLE);

	if (initVerify(window, renderer) == false) {
		return -1;
	}

	// Texturas, Surfaces... 
	SDL_Surface* surfaceImage = IMG_Load("mario_sprite.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surfaceImage);
	
	SDL_Surface* surfaceImageInvertida = IMG_Load("mario_sprite_invertido.png");
	SDL_Texture* textureInvertida = SDL_CreateTextureFromSurface(renderer, surfaceImageInvertida);

	SDL_Surface* surfaceYoshIsland = IMG_Load("yosh_island.png");
	SDL_Texture* textureYoshIsland = SDL_CreateTextureFromSurface(renderer, surfaceYoshIsland);

	SDL_Surface* surfaceMouseCursor = IMG_Load("mouse_cursor.png");
	SDL_Texture* textureMouseCursor = SDL_CreateTextureFromSurface(renderer, surfaceMouseCursor);

	// Rect.
	SDL_Rect rect = {100, 100, 40, 40};
	Personagem player = Personagem(100, 504 - 64, textureInvertida);
	player.spriteClipRect = { 964, 48, 16, 24 };

	// Game Main Loop.
	bool running = true;
	unsigned int a = SDL_GetTicks();
	unsigned int b = SDL_GetTicks();
	double delta = 0;

	int cameraWidth = 128;  // <-- antes tava setado como 128.
	int cameraHeight = 128;
	SDL_Rect cameraRect;

	cameraRect.w = cameraWidth + player.rect.w;
	cameraRect.h = cameraHeight + player.rect.h;

	std::vector<SDL_Rect> walking_animation = { {180, 48, 16, 24},
												{232, 48, 16, 24},
												{284, 48, 16, 24}};
	
	bool isMarioWalking = false;


	// 32, 48
	SDL_Rect tileClipRect{32, 384, 32, 48};

	std::vector<SDL_Rect> map{};

	//add tiles in map.
	for (int i = 0; i < 14; i++) {

		map.push_back(SDL_Rect{64 * i, 504, 64, 96});
	}

	while (running)
	{

		SDL_Event event;

		while (SDL_PollEvent(&event) != 0) {

			switch (event.type)
			{
			case (SDL_QUIT):
				running = false;
				break;

			default:
				break;
			}
		}

		a = SDL_GetTicks();
		delta = a - b;

		SDL_GetMouseState(&mouseX, &mouseY);

		//std::cout << "MouseX: " << mouseX;
		//std::cout << "\nMouseY: " << mouseY;
		
		// Controlling FrameRate. (FPS).
		if (delta > 1000 / 60.0)
		{
			//std::cout << "fps: " << 1000 / delta << std::endl;

			// Camera. division by 2 - the player stay on center.
			cameraRect.x = player.rect.x - cameraWidth / 2;
			cameraRect.y = player.rect.y - cameraHeight / 2;

			SDL_SetRenderDrawColor(renderer, 140, 120, 230, 255);
			SDL_RenderClear(renderer);

			// Movement, Keyboard Input.
			playerCheckMovement(&player, texture, textureInvertida); // playerCheckMovement deve vir antes do metodo setAnimation.
			player.setAnimation();

			SDL_RenderCopy(renderer, player.texture, &player.spriteClipRect, &player.rect);
			
			// Draw custom mouse cursor.
			SDL_RenderCopy(renderer, textureMouseCursor, NULL, new SDL_Rect{mouseX - 16, mouseY, 32, 32});

			// Drawing map from vector.
			for (int i = 0; i < map.size(); i++) {

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderDrawRect(renderer, new SDL_Rect{ map.at(i).x, 100, 100, 100 });

				SDL_RenderCopy(renderer, textureYoshIsland, &tileClipRect, &map.at(i));
			}

			// Drawing outline camera rect.
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &cameraRect);

			// Updating tile's position according to camera's x.
			int qntExcedida = 0;
			for(int i =0; i < map.size(); i++){

				std::cout << "TILE " << i << " - X: " << map.at(i).x << " - Y: " << map.at(i).y << " - calc : " << cameraRect.x + cameraWidth << " - isWalking: " << player.isWalking << "\n";
				//std::cout << "Player X: " << player.rect.x << "\n";
				
				if (cameraRect.x + cameraRect.w > WINDOW_WIDTH) {
	
					qntExcedida = cameraRect.x + cameraRect.w - (WINDOW_WIDTH);

					//map.at(i).x -= map.at(i).x + cameraRect.x + cameraWidth - WINDOW_WIDTH;

					if (player.isWalking) {
						map.at(i).x -= player.speed;
					}
					
				}
				else if (cameraRect.x < 0) {
					//map.at(i).x = map.at(i).x + (WINDOW_WIDTH - cameraRect.x);
					qntExcedida = abs(player.rect.x);
					
					if (player.isWalking) {
						map.at(i).x += player.speed;
					}
					
				}
				std::cout << "excedido: " << qntExcedida << "\n";
			}

			// Verifing if camera's position excede bounds of screen.
			if (cameraRect.x + cameraRect.w > WINDOW_WIDTH) {
				if (player.isWalking && player.direction == 1) {
					player.rect.x = WINDOW_WIDTH - cameraWidth + qntExcedida;
				}
			}
			else if (cameraRect.x < 0) {
				if (player.isWalking && player.direction == -1) {
					player.rect.x = cameraWidth / 2 - 2;
				}
			}

			SDL_RenderPresent(renderer);
		
			delta = 0;
			b = SDL_GetTicks();
		}
	}
	return 0;
}

bool initVerify(SDL_Window* window, SDL_Renderer* renderer){
	if (window == nullptr) {
		SDL_Log("Could not create a window: %s", SDL_GetError());
		return false;
	}else if (renderer == nullptr) {
		SDL_Log("Could not create the renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

void playerCheckMovement(Personagem *personagem, SDL_Texture* textura1, SDL_Texture* textura2) {
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	if (currentKeyStates[SDL_SCANCODE_W]) {
		personagem->rect.y -= personagem->speed;
	}
	else if (currentKeyStates[SDL_SCANCODE_A])
	{
		personagem->rect.x -= personagem->speed;
		personagem->setSprite(spritesEnum::MarioLeft);
		personagem->texture = textura1;
		personagem->direction = -1;
		personagem->isWalking = true;
	}
	else if (currentKeyStates[SDL_SCANCODE_S])
	{
		personagem->rect.y += personagem->speed;
	}
	else if (currentKeyStates[SDL_SCANCODE_D])
	{
		personagem->rect.x += personagem->speed;
		personagem->setSprite(spritesEnum::MarioRight);
		personagem->texture = textura2;
		personagem->direction = 1;
		personagem->isWalking = true;
	}
	else {
		personagem->isWalking = false;
	}
}