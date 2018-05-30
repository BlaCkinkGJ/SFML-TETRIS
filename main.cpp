#include<iostream>
#include<windows.h>
#include<random>
#include<iomanip>

#include<SFML/Graphics.hpp>

#include "tetris.hpp"

using namespace std;

void drawImage(sf::RenderWindow& window, const sf::Texture& texture, Tetris& tetris, const int color);

int main(void){
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Tetris");
	sf::Texture texture;
	if (!texture.loadFromFile("image.png")) {
		return -1;
	}
	window.setFramerateLimit(24);

	Board data;
	int block[data.row * data.col];
	Tetris tetris(12, 16);
	Key key = Key::Nothing;
	sf::Clock clock;

	mt19937 seed(time(NULL));
	uniform_int_distribution<int> range(0, 6);

	unsigned int color = range(seed);
	memcpy(data.shape[color], block, data.row * data.col);

	tetris.genBlock(block);
	while (window.isOpen()) {
		
		sf::Event event;
		sf::Time counter = clock.getElapsedTime();
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			key = Key::Right;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			key = Key::Left;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			key = Key::Down;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			key = Key::Rotate;
		if (counter.asMilliseconds() > 300.0f) {
			tetris.update(key);
			if (tetris.getExist() == false) {
				color = range(seed);
				memcpy(data.shape[color], block, data.row * data.col);
				tetris.genBlock(block);
			}
		key = Key::Nothing;
			clock.restart();
		}
		window.clear();
		drawImage(window, texture, tetris, color);
		window.display();
		if (tetris.getGameOver()) {
			cout << "Game Over" << endl;
			cout << "Escape 키를 눌러주십시오." << endl;
			while (1) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					window.close();
					break;
				}
			}
		}
	}
    return 0;
}

void drawImage(sf::RenderWindow& window, const sf::Texture& texture, Tetris& tetris, const int color) {
	const int *board = tetris.getBoard();

	sf::Sprite sprite;
	sprite.setTexture(texture);

	Point start = { 10, 10 };
	const int pixel = 50;
	const float resize = 0.5f;
	const int *block = tetris.getBlock();
	Point pos = tetris.getPos();

	sprite.scale(sf::Vector2f(resize, resize));

	int gap = pixel * resize;
	for (int y = 1; y < tetris.getHeight(); y++) {
		for (int x = 0; x < tetris.getWidth(); x++) {
			int value = board[y*tetris.getWidth() + x];
			if (value > 0) {
				// 주의할 것 3, 4 이런 숫자는 내가 맘대로 넣은 거임
				bool cmpX = (x >= pos.x && x <= pos.x + 3);
				bool cmpY = (y >= pos.y && y <= pos.y + 3);
				if (value != 3 && cmpX && cmpY && (block[(y - pos.y) * 4 + (x - pos.x)] > 0)) {
					sprite.setTextureRect(sf::IntRect(pixel*color, 0, 50, 50));
				}
				else {
					if (value == 3)
						sprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
					else
						sprite.setTextureRect(sf::IntRect(50, 0, 50, 50));
				}
				sprite.setPosition(sf::Vector2f(start.x + (gap*x), start.y + (gap*y))); // absolute position
				window.draw(sprite);
			}
		}
	}
}
