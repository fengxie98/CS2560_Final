// SFML Template.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <cstdlib>
#include <vector>

class Laser
{
public:
	sf::Sprite beam;

	Laser(sf::Texture *texture, sf::Vector2f pos)
	{
		this->beam.setTexture(*texture);

		this->beam.setScale(0.05f, 0.05f);

		this->beam.setPosition(pos);

		this->beam.setRotation(270.f);
	}

	~Laser(){}
};

class Player
{
public:
	sf::Sprite ship;
	sf::Texture *texture;
	int HP;
	int HPMax;

	std::vector<Laser> lasers;

	Player(sf::Texture *texture)
	{
		this->HPMax = 10;
		this->HP = this->HPMax;

		this->texture = texture;
		this->ship.setTexture(*texture);

		this->ship.setScale(0.1f, 0.1f);
	}

	~Player()
	{}
};

class Enemy
{
public:
	sf::Sprite Eship;

	int HP;
	int HPMAX;

	Enemy(sf::Texture *texture, sf::Vector2u windowSize)
	{
		this->HPMAX = rand() % 3 + 1; //1 to 3
		this->HP = this->HPMAX;

		this->Eship.setTexture(*texture);

		this->Eship.setScale(0.1f, 0.1f);

		this->Eship.setPosition(windowSize.x - this->Eship.getGlobalBounds().width, rand()%windowSize.y - this->Eship.getGlobalBounds().height);

		this->Eship.setRotation(180.f);
	}

	~Enemy()
	{
	
	}

};

int main()
{
	srand(time(NULL));

	std::cout << "Hello World!\n";
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Space Shooter!");
	window.setFramerateLimit(60);

	//init text
	sf::Font font;
	font.loadFromFile("Fonts/Supreme.ttf");

	//init textures
	sf::Texture playerTex;
	playerTex.loadFromFile("Textures/ship.png");

	sf::Texture enemyTex;
	enemyTex.loadFromFile("Textures/enemy.png");

	sf::Texture laserTex;
	laserTex.loadFromFile("Textures/laser.png");


	//Player init
	Player player(&playerTex);
	int shootTimer = 20;
	//shoot timer

	//Enemy init
	int enemySpawnTimer = 0;
	std::vector<Enemy> enemies;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Player
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			player.ship.move(0.f, -10.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			player.ship.move(-10.f, 0.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			player.ship.move(10.f, 0.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			player.ship.move(0.f, 10.f);

		//Check collision with window
		if (player.ship.getPosition().x <= 0) //Left of screen
			player.ship.setPosition(0.f, player.ship.getPosition().y);
		if (player.ship.getPosition().x >= window.getSize().x - player.ship.getGlobalBounds().width) //Right of screen
			player.ship.setPosition(window.getSize().x - player.ship.getGlobalBounds().width, player.ship.getPosition().y);
		if (player.ship.getPosition().y <= 0) //Top of screen
			player.ship.setPosition(player.ship.getPosition().x, 0.f);
		if (player.ship.getPosition().y >= window.getSize().y - player.ship.getGlobalBounds().height) //Bottom of screen
			player.ship.setPosition(player.ship.getPosition().x, window.getSize().y - player.ship.getGlobalBounds().height);

		//update controls
		if (shootTimer < 20)
		{
			shootTimer++;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer >= 20) //shooting
		{
			player.lasers.push_back(Laser(&laserTex, player.ship.getPosition()));
			shootTimer = 0; //reset Timer
		}

		//Lasers
		for (size_t i = 0; i < player.lasers.size(); i++)
		{
			//Move
			player.lasers[i].beam.move(20.f, 0.f);

			if (player.lasers[i].beam.getPosition().x > window.getSize().x)
			{
				player.lasers.erase(player.lasers.begin() + i);
				break;
			}

			//Enemy Collision
			for (size_t k = 0; k < enemies.size(); k++)
			{
				if (player.lasers[i].beam.getGlobalBounds().intersects(enemies[k].Eship.getGlobalBounds()))
				{
					enemies.erase(enemies.begin() + k);
					player.lasers.erase(player.lasers.begin() + i);
					break; 
				}
			}
		}


		//enemy
		if(enemySpawnTimer < 20)
			enemySpawnTimer++;
		
		//enenmy spawn
		if (enemySpawnTimer >= 20)
		{
			enemies.push_back(Enemy(&enemyTex, window.getSize()));
			enemySpawnTimer = 0; //reset timer
		}

		for (size_t i = 0; i < enemies.size(); i++)
		{
			enemies[i].Eship.move(-5.f, 0.f);

			if (enemies[i].Eship.getPosition().x <= 0 - enemies[i].Eship.getGlobalBounds().width)
			{
				enemies.erase(enemies.begin() + i);
			}

			if (enemies[i].Eship.getGlobalBounds().intersects(player.ship.getGlobalBounds()))
			{
				enemies.erase(enemies.begin() + i);

				player.HP--; //player takes damage when player crashes into enemy
			}
		}

			//draw
			window.clear();

			//player
			window.draw(player.ship);

			//lasers
			for (size_t i = 0; i < player.lasers.size(); i++)
			{
				window.draw(player.lasers[i].beam);
			}

			//enemy
			for (size_t i = 0; i < enemies.size(); i++)
			{
				window.draw(enemies[i].Eship);
			}

			window.display();
	}
	return 0;
}
