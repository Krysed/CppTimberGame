#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
//for debug
#include <iostream>

const int NUM_OF_BRANCHES = 6;

void updateBranches(int randomSeed);

enum class side { LEFT, RIGHT, NONE};


//array of branches
sf::Sprite branches[ NUM_OF_BRANCHES ];
side branchPositions[ NUM_OF_BRANCHES ];


int main()
{
	//window params
	const int WIDTH		= 1920;
	const int HEIGHT	= 1080;
	//game options
	bool bIsGamePaused = true;

	//init pseudo-random num;
	srand(time(NULL));

	//game start options
	sf::Font font;
	sf::Text scoreText;
	sf::Text messageText;
	int playerScore		= 0;

	float beeSpeed		= 0.0f;
	float cloudSpeed1	= 0.0f;
	float cloudSpeed2	= 0.0f;
	float cloudSpeed3	= 0.0f;

	bool bBeeActive		= false;
	bool bCloud1Active	= false;
	bool bCloud2Active	= false;
	bool bCloud3Active	= false;

	
	//initializing time
	sf::Clock clock;
	sf::RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;

	timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition((WIDTH / 2) - (timeBarStartWidth / 2), HEIGHT - 100);

	//Time tracking
	sf::Time getTimeTotal;
	float timeRemaining = 6.f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//Window render
	sf::VideoMode vm(WIDTH, HEIGHT);
	sf::RenderWindow window(vm, "Timber!", sf::Style::Fullscreen);

	
	sf::Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	if(textureBackground.loadFromFile("graphics/background.png") == NULL)
	{
		return 0;
	}

	//setting up background
	sf::Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	//loading font
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	messageText.setFont(font);
	scoreText.setFont(font);

	//setting text
	messageText.setString("Press ENTER to start");
	scoreText.setString("Score: ");

	//character size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//character color
	messageText.setFillColor(sf::Color::White);
	scoreText.setFillColor(sf::Color::White);

	sf::FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(
		textRect.left +
		textRect.width / 2.f,
		textRect.top +
		textRect.height / 2.f
	);
	messageText.setPosition(WIDTH / 2.f, HEIGHT / 2.f);
	scoreText.setPosition(50, 50);

	//setting up branches
	sf::Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	for (int i = 0; i < NUM_OF_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	//setting up tree
	sf::Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
		
	sf::Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810,0);

	//setting up bee
	sf::Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	sf::Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 700);

	//setting up Clouds
	sf::Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");

	sf::Sprite spriteCloud1;
	sf::Sprite spriteCloud2;
	sf::Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);


	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(3);

	/**
	 * @brief game loop 
	*/
	while (window.isOpen())
	{
		window.clear();

		//drawing sprites
		window.draw(spriteBackground);

		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		window.draw(spriteTree);
		window.draw(spriteBee);
		
		std::stringstream ss;
		ss << "Score : " << playerScore;
		scoreText.setString(ss.str());
		
		//positioning branches
		for (int i = 0; i < NUM_OF_BRANCHES; i++)
		{
			float height = i * 150;
			if (branchPositions[i] == side::LEFT)
			{
				branches[i].setPosition(610, height);
				//rotate branch 180 degrees
				branches[i].setRotation(180);
			}
			else if (branchPositions[i] == side::RIGHT)
			{
				branches[i].setPosition(1330, height);
				//reset branch rotation
				branches[i].setRotation(0);
			}
			else
			{
				//hide branch
				branches[i].setPosition(3000, height);
			}
		}

		//drawing text
		if (bIsGamePaused)
		{
			window.draw(messageText);
		}
		else
		{
			window.draw(timeBar);
			window.draw(scoreText);

		}
		for (int i = 0; i < NUM_OF_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}
		window.display();

		if(!bIsGamePaused)
		{
			//measure time
			sf::Time dtime = clock.restart();

			//subtract from the amount of time remaining
			timeRemaining -= dtime.asSeconds();
			//size the time bar
			timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
			
			if (timeRemaining <= 0)
			{
				bIsGamePaused = true;

				messageText.setString("Out of Time!");

				sf::FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
				messageText.setPosition(WIDTH / 2.f, HEIGHT / 2.f);
			}

			if (!bBeeActive)
			{
				//how fast is bee moving
				beeSpeed = (rand() % 200) + 200;

				//how hight is the bee
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);

				bBeeActive = true;
			}
			//moving the bee
			else
			{
				spriteBee.setPosition(
					spriteBee.getPosition().x - (beeSpeed * dtime.asSeconds()),
					spriteBee.getPosition().y
				);
				if (spriteBee.getPosition().x < -100)
				{
					bBeeActive = false;
				}
			}
			//moving clouds
			if (!bCloud1Active)
			{
				//cloud speed
				srand(time(NULL) * 10);
				cloudSpeed1 = rand() % 200;

				//cloud height
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				bCloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x + (cloudSpeed1 * dtime.asSeconds()),
					spriteCloud1.getPosition().y
				);
				//is cloud out of bounds
				if (spriteCloud1.getPosition().x > 1920)
				{
					bCloud1Active = false;
				}
			}
			if (!bCloud2Active)
			{
				//cloud speed
				srand(time(NULL) * 20);
				cloudSpeed2 = rand() % 200;

				//cloud height
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				bCloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x + (cloudSpeed2 * dtime.asSeconds()),
					spriteCloud2.getPosition().y
				);
				//is cloud out of bounds
				if (spriteCloud2.getPosition().x > 1920)
				{
					bCloud2Active = false;
				}
			}
			//moving clouds
			if (!bCloud3Active)
			{
				//cloud speed
				srand(time(NULL) * 30);
				cloudSpeed3 = rand() % 200;

				//cloud height
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				bCloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x + (cloudSpeed3 * dtime.asSeconds()),
					spriteCloud3.getPosition().y
				);
				//is cloud out of bounds
				if (spriteCloud3.getPosition().x > 1920)
				{
					bCloud3Active = false;
				}
			}
		}

		//Handling player input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			if (bIsGamePaused)bIsGamePaused = false;
			playerScore = 0;
			timeRemaining = 6.f;		}
	}
	return 0;
}

void updateBranches(int randomSeed)
{
	for (int i = NUM_OF_BRANCHES; i > 0; i--)
	{
		branchPositions[i] = branchPositions[i - 1];
	}
	srand(time(NULL));
	
	int random = (rand() % 5 + randomSeed);
	switch (random)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}