/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
const int amountOfTiles = 200;
class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();

	void setupTiles();
	
	void init();
	void collisionDetection();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message

	sf::CircleShape player;

	sf::RectangleShape tiles[amountOfTiles];
	sf::RectangleShape playerMovableBox;
	bool playing = false;

	int row = 0;
	int column = 0;
	int levelData[amountOfTiles]
	{
		0,0,0,1,1,1,1,1,1,1,
		1,0,0,1,1,1,1,1,1,1,
		1,0,0,0,1,1,1,1,1,1,
		1,1,0,0,1,1,1,1,1,1,
		1,1,0,0,0,1,1,1,1,1,
		1,1,1,0,0,0,1,1,1,1,
		1,1,1,1,0,0,0,1,1,1,
		1,1,1,0,0,1,1,1,1,1,
		1,1,0,0,1,1,1,1,1,1,
		1,0,0,0,1,1,1,1,1,1,
		1,0,0,1,1,1,1,1,1,1,
		1,1,0,0,1,1,1,1,1,1,
		1,1,0,0,0,1,1,1,1,1,
		1,1,1,0,0,1,1,1,1,1,
		1,1,0,0,0,1,1,1,1,1,
		1,1,1,0,0,0,1,1,1,1,
		1,1,1,1,0,0,0,1,1,1,
		1,1,1,1,0,0,1,1,1,1,
		1,1,1,0,0,0,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0
	};

	bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

