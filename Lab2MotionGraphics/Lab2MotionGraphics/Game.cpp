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
///  

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
	else if (sf::Keyboard::Space == t_event.key.code && player.getFillColor() == sf::Color::Green)
	{
		init();
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
			tiles[index].setPosition(sf::Vector2f(tiles[index].getPosition().x, tiles[index].getPosition().y + speedOfTiles));
		}
		if (player.getGlobalBounds().intersects(playerMovableBox.getGlobalBounds()))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				player.setPosition(player.getPosition().x, player.getPosition().y - playerSpeed);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			player.setPosition(player.getPosition().x - playerSpeed, player.getPosition().y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			player.setPosition(player.getPosition().x + playerSpeed, player.getPosition().y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			player.setPosition(player.getPosition().x, player.getPosition().y + playerSpeed);
		}



		for (int i = 0; i < amountOfProjectiles; i++)
		{
			if (projectiles[i].getPosition().x != offscreen.x)
			{
				projectiles[i].move(0, -speedOfProjectile);

				if (projectiles[i].getPosition().y < 0)
				{
					projectiles[i].setPosition(offscreen);
				}
			}
		}
	
		for (int i = 0; i < amountOfProjectiles; i++)
		{
			if (enemyProjectiles[i].getPosition().x != offscreen.x)
			{
				enemyProjectiles[i].move(enemySpeedOfProjectile, speedOfTiles);

				if (enemyProjectiles[i].getPosition().x < 0||enemyProjectiles[i].getPosition().x>m_window.getSize().x)
				{
					enemyProjectiles[i].setPosition(offscreen);
				}
			}
		}

			int waitToFireInterval = waitToFireCounter;

			if (readyToFire == true)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
				{
					for (int i = 0; i < amountOfProjectiles; i++)
					{
						if (projectiles[i].getPosition() == offscreen)
						{
							projectiles[i].setPosition(player.getPosition());
							readyToFire = false;
							waitToFireCounter = waitToFireInterval;
							break;
						}

					}
				}
			}
			else {
				waitToFireCounter--;
				if (waitToFireCounter <= 0)
				{
					readyToFire = true;
				}
			}

			scoreText.setString(std::to_string(m_score));


	}

	gameoverText.setString("");
	collisionDetection();
	if (tiles[0].getPosition().y > m_window.getSize().y)
	{
		gameWon = true;
	}


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
	for (int i = 0; i < amountOfProjectiles; i++)
	{
		m_window.draw(projectiles[i]);
		m_window.draw(enemyProjectiles[i]);
	
	}

	if (gameWon)
	{
		gameoverText.setString("Game Over, You Won!");
		gameoverText.setPosition(100, 400);
	}
	m_window.draw(gameoverText);
	m_window.draw(scoreText);
	m_window.draw(player);
	m_window.display();
}

void Game::setupTiles()
{
	for (int index = 0; index < amountOfTiles; index++)
	{
		
		tiles[index].setSize(sf::Vector2f(80.0f, 80.0f));
		tiles[index].setPosition(column * 80, row * 80 - (amountOfTiles*7.5));
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
		else if (levelData[index]==0)
		{
			if (rand() % 50 == 0)
			{
				tiles[index].setFillColor(sf::Color::Blue);
			}
			else
			{
				tiles[index].setFillColor(sf::Color::Transparent);
			}
			
		}
		else
		{
			tiles[index].setFillColor(sf::Color::Yellow);
		}
	}

}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::init()
{
	gameoverText.setCharacterSize(45U);
	gameoverText.setFillColor(sf::Color::Cyan);
	gameoverText.setFont(m_ArialBlackfont);
	gameoverText.setPosition(50, 400);

	scoreText.setCharacterSize(25U);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setFont(m_ArialBlackfont);
	scoreText.setPosition(50, 50);

	row = 0;
	column = 0;
	playing = false;
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

	for (int i = 0; i < amountOfProjectiles; i++)
	{
		projectiles[i].setRadius(3.0f);
		projectiles[i].setFillColor(sf::Color::Cyan);
		projectiles[i].setPosition(offscreen);
		projectiles[i].setOrigin(sf::Vector2f(projectiles[i].getRadius(), projectiles[i].getRadius()));

		enemyProjectiles[i].setRadius(5.0f);
		enemyProjectiles[i].setFillColor(sf::Color::Magenta);
		enemyProjectiles[i].setPosition(offscreen);
		enemyProjectiles[i].setOrigin(sf::Vector2f(enemyProjectiles[i].getRadius(), enemyProjectiles[i].getRadius()));

	}
	screenBounds.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y));
	speedOfTiles = 4;
	playerSpeed = 3.5f;
	m_score = 0;
}

void Game::collisionDetection()
{
	for (int index = 0; index < amountOfTiles; index++)
	{
		if (tiles[index].getFillColor() == sf::Color::Blue)
		{
			if (tiles[index].getGlobalBounds().intersects(player.getGlobalBounds()))
			{
				tiles[index].setFillColor(sf::Color::Transparent);
				effectRandomiser();
				m_score += 10;
			}
		}
		if (tiles[index].getFillColor()==sf::Color::Yellow)
		{
			for (int i = 0; i < amountOfProjectiles; i++)
			{
				if (projectiles[i].getGlobalBounds().intersects(tiles[index].getGlobalBounds()))
				{
					tiles[index].setFillColor(sf::Color::Black);
				}
			}
			for (int i = 0; i < amountOfProjectiles; i++)
				{
					int waitToFireInterval = enemyWaitToFireCounter;
					if (enemyReadyToFire == true)
					{
						if (tiles[index].getGlobalBounds().intersects(screenBounds.getGlobalBounds()))
						{
							if (levelData[index] == 2)
							{
								enemySpeedOfProjectile = 5;
							}
							else
							{
								enemySpeedOfProjectile = -5;
							}
							for (int i = 0; i < amountOfProjectiles; i++)
							{
								if (enemyProjectiles[i].getPosition() == offscreen)
								{
									enemyProjectiles[i].setPosition(tiles[index].getPosition());
									
									enemyReadyToFire = false;
									enemyWaitToFireCounter = waitToFireInterval;
									break;
								}

							}
						}
					}
					else {
						enemyWaitToFireCounter--;
						if (enemyWaitToFireCounter <= 0)
						{
							enemyReadyToFire = true;
						}
					}
				}
			
		}
		if (levelData[index] == 1)
		{
			if (player.getGlobalBounds().intersects(tiles[index].getGlobalBounds()))
			{
				player.setFillColor(sf::Color::Green);
				gameoverText.setString("You Lose! Space to Restart");
				playing = false;
			}
		}
	}

	if (player.getPosition().y  > m_window.getSize().y - player.getRadius())
	{
		player.setPosition(player.getPosition().x, m_window.getSize().y - player.getRadius());
	}
	for (int i = 0; i < amountOfProjectiles; i++)
	{
		if (player.getGlobalBounds().intersects(enemyProjectiles[i].getGlobalBounds()))
		{
			player.setFillColor(sf::Color::Green);
			gameoverText.setString("You Lose! Space to Restart");
			playing = false;
		}
	
	}
	
	
}

void Game::effectRandomiser()
{
	switch (rand() % 3)
	{
	case 0:
		playerSpeed += 2;
		std::cout << "Player speed increased" << std::endl;
		break;
	case 1:
		if(speedOfTiles>2)
		speedOfTiles -= 0.5;

		std::cout << "Speed of tiles decreased" << std::endl;
		break;
	case 2:
		player.setRadius(3);
		player.setOrigin(sf::Vector2f(player.getRadius(), player.getRadius()));
		std::cout << "player size decreased" << std::endl;
		break;
	default:
		break;
	}
}
