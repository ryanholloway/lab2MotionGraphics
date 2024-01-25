/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 800U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	init();
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}

	if (sf::Keyboard::Space == t_event.key.code&&player.getFillColor()!= sf::Color::Green)
	{
		playing = !playing;
	}



	
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (playing)
	{
		for (int index = 0; index < amountOfTiles; index++)
		{
			tiles[index].setPosition(sf::Vector2f(tiles[index].getPosition().x, tiles[index].getPosition().y + 1));
		}
		if (player.getGlobalBounds().intersects(playerMovableBox.getGlobalBounds()))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				player.setPosition(player.getPosition().x, player.getPosition().y - 5);
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			player.setPosition(player.getPosition().x - 5, player.getPosition().y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			player.setPosition(player.getPosition().x + 5, player.getPosition().y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			player.setPosition(player.getPosition().x, player.getPosition().y + 5);
		}
	}
	
	collisionDetection();
	if (m_exitGame)
	{
		m_window.close();
	}
	
	
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);
	for (int index = 0; index < amountOfTiles; index++)
	{
		m_window.draw(tiles[index]);
	}
	m_window.draw(player);
	m_window.display();
}

void Game::setupTiles()
{
	for (int index = 0; index < amountOfTiles; index++)
	{
		
		tiles[index].setSize(sf::Vector2f(80.0f, 80.0f));
		tiles[index].setPosition(column * 80, row * 80 - (amountOfTiles/2.0f*8));
		column++;

		if (column == 10)
		{
			column = 0;
			row++;
		}
	
		if (levelData[index] == 1)
		{
			tiles[index].setFillColor(sf::Color::Red);

		}
		else
		{
			tiles[index].setFillColor(sf::Color::Transparent);
		}
	}

}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::init()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	setupTiles();
	player.setRadius(10.0f);
	player.setOrigin(sf::Vector2f(player.getRadius(), player.getRadius()));
	player.setFillColor(sf::Color::Blue);
	player.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y - 40);

	playerMovableBox.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y / 4.0f));
	playerMovableBox.setPosition(0, m_window.getSize().y - m_window.getSize().y / 4.0f);
	playerMovableBox.setFillColor(sf::Color::Green);

}

void Game::collisionDetection()
{
	for (int index = 0; index < amountOfTiles; index++)
	{
		if (tiles[index].getFillColor() == sf::Color::Red)
		{
			if (player.getGlobalBounds().intersects(tiles[index].getGlobalBounds()))
			{
				player.setFillColor(sf::Color::Green);
				playing = false;
			}
		}
	}

	if (player.getPosition().y  > m_window.getSize().y - player.getRadius())
	{
		player.setPosition(player.getPosition().x, m_window.getSize().y - player.getRadius());
	}
	
}
