#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <Windows.h>
class Resources
{
public:
	sf::Font font;
	sf::Texture bg;
	sf::Texture playerLeft;
	sf::Texture playerRight;
	sf::SoundBuffer punch;
	sf::Sound sound;
	sf::Music music;
	sf::Music musicMenu;
	void loadResources()
	{
		
		font.loadFromFile("fonts/badcomic.ttf");
		bg.loadFromFile("textures/bg.png");
		playerLeft.loadFromFile("textures/player1.png");
		playerRight.loadFromFile("textures/player2.png");
		
		punch.loadFromFile("sound/punch.wav");
		sound.setBuffer(punch);
		sound.setVolume(20.f);

		music.openFromFile("sound/music.wav");
		music.setLoop(true);
		music.setVolume(0.f);
		music.play();

		musicMenu.openFromFile("sound/music_menu.wav");
		musicMenu.setLoop(true);
		musicMenu.setVolume(50.f);
		musicMenu.play();
	}
	void loadMenuMusic()
	{
		musicMenu.setVolume(50.f);
		music.setVolume(0);
	}
	void loadGameMusic()
	{
		musicMenu.setVolume(0);
		music.setVolume(50.f);
	}
};
class drawShapes
{
public:
	sf::FloatRect drawRectangleOutline(sf::RenderWindow& window, sf::Vector2f size, sf::Color color, float x, float y, float thickness)
	{
		sf::RectangleShape rectangle;
		rectangle.setSize(size);
		rectangle.setPosition(x, y);
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(color);
		rectangle.setOutlineThickness(thickness);
		window.draw(rectangle);
		return rectangle.getGlobalBounds();
	}
	void drawRectangle(sf::RenderWindow& window, sf::Vector2f size, sf::Color color, float x, float y)
	{
		sf::RectangleShape rectangle;
		rectangle.setSize(size);
		rectangle.setPosition(x, y);
		rectangle.setFillColor(color);
		window.draw(rectangle);
	}
	void drawCircle(sf::RenderWindow& window, float size, sf::Color color, float x, float y)
	{
		sf::CircleShape circle;
		circle.setRadius(size);
		circle.setPosition(x, y);
		circle.setFillColor(color);
		window.draw(circle);
	}
	void drawTriangle(sf::RenderWindow& window, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Color color)
	{
		sf::ConvexShape convex;
		convex.setPointCount(3);
		convex.setPoint(0, p1);
		convex.setPoint(1, p2);
		convex.setPoint(2, p3);
		convex.setFillColor(color);
		window.draw(convex);
	}
	void drawText(sf::RenderWindow& window, float x, float y, int size, std::string data, sf::Color color, Resources& resourcesHolder)
	{
		sf::Text text;
		text.setFont(resourcesHolder.font);
		text.setString(data);
		text.setFillColor(color);
		text.setCharacterSize(size);
		text.setPosition(x, y);
		sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(bounds.width / 2.f, 0);
		window.draw(text);
	}
};

class drawSprites
{
public:
	void drawSprite(sf::RenderWindow& window, sf::Texture texture, float x, float y)
	{
		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprite.setPosition(x, y);
		window.draw(sprite);
	}
};
class Player
{

public:
	int playerPosition = 0;
	int score = 0;
	int highScore = 0;
	float health = 100;
	short branches[5];
	bool gameOver = false;
	int size;
	void drawPlayer(sf::RenderWindow& window, drawSprites& drawSprites, Resources& resourcesHolder)
	{
		if (health <= 0)
		{
			std::cout << "time out, your score:" << score << std::endl;
			if (score > highScore)
			{
				highScore = score;
				std::cout << "new high score" << std::endl;
			}
			gameOver = true;
		}
		resourcesHolder.playerRight.loadFromFile("textures/player2.png"); //no idea why player2.png glitches out without it
		if (playerPosition == 0)
		{
			drawSprites.drawSprite(window, resourcesHolder.playerLeft, 410, 400);
		}
		else if (playerPosition == 1)
		{
			drawSprites.drawSprite(window, resourcesHolder.playerRight, 710, 400);
		}

	}
	void play(int position)
	{
		if (position == branches[size - 2])
		{
			std::cout << "you died, your score:" << score << std::endl;
			if (score > highScore)
			{
				highScore = score;
				std::cout << "new high score" << std::endl;
			}
			gameOver = true;
			return;
		}
		if (position == 0)
		{
			playerPosition = 0;
			if (health < 100)
				health += 20;
		}
		else if (position == 1)
		{
			playerPosition = 1;
			if (health < 100)
				health += 20;
		}
		score ++;
		shiftBranches();

	}
	void shiftBranches()
	{
		for (int i = size - 1; i >= 0; i--)
		{
			if (i == 0)
				branches[i] = rand() % 2;
			else
				branches[i] = branches[i - 1];
		}
	}
	void startGame()
	{
		size = sizeof(branches) / sizeof(branches[0]);
		for (int i = 0; i < size; i++)
		{
			if (i == size - 1)
				branches[i] = 2;
			else
				branches[i] = rand() % 2;

			printf("player.branches[%d] = %d\n", i, branches[i]);
		}
		health = 100;
		score = 0;
	}
};


class Scenes
{

public:
	unsigned int playerPositon = 0;
	bool menu = true;
	sf::FloatRect boxStart;
	sf::FloatRect boxQuit;
	sf::FloatRect boxOk;
	void drawGameplay(sf::RenderWindow& window, drawSprites& drawSprites, Player& player, drawShapes& drawShapes, Resources& resourcesHolder)
	{
		if (player.gameOver == true)
		{
			menu = true;
			player.gameOver = false;
			return;
		}
		drawShapes.drawRectangle(window, sf::Vector2f(100, 640), sf::Color::Color(60, 30, 10), 590, 0);
		drawShapes.drawRectangle(window, sf::Vector2f(player.health * 12.8, 25), sf::Color::Blue, 0, 700);
		player.drawPlayer(window, drawSprites, resourcesHolder);
		if (player.health >= 0)
			player.health = player.health - (1 + 0.002*player.score);
		for (int i = 0; i < player.size; i++)
		{
			//printf("branches[%d] = %d\n", i, player.branches[i]);
			if (player.branches[i] == 0)
				drawShapes.drawRectangle(window, sf::Vector2f(200, 50), sf::Color::Color(60, 30, 10), 395, 50 + (i * 100));
			else if(player.branches[i] == 1)
				drawShapes.drawRectangle(window, sf::Vector2f(200, 50), sf::Color::Color(60, 30, 10), 685, 50 + (i * 100));
		}
		drawShapes.drawText(window, 630, 20, 114, std::to_string(player.score), sf::Color::Red, resourcesHolder);
	}
	void drawGame(sf::RenderWindow& window, drawSprites& drawSprites, Player& player, drawShapes& drawShapes, Resources& resourcesHolder)
	{
		drawSprites.drawSprite(window, resourcesHolder.bg, 0, 0);
		if (menu == true)
			drawMenu(window, drawSprites, drawShapes, resourcesHolder);
		else if (player.gameOver == true)
			drawGameOver(window, drawSprites, drawShapes, resourcesHolder, player);
		else
			drawGameplay(window, drawSprites, player, drawShapes, resourcesHolder);
	}
	void drawMenu(sf::RenderWindow& window, drawSprites& drawSprites, drawShapes& drawShapes, Resources& resourcesHolder)
	{
		drawShapes.drawText(window, 640, 0, 114, "narodowy drwal", sf::Color::Red, resourcesHolder);
		boxStart = drawShapes.drawRectangleOutline(window, sf::Vector2f(500, 150), sf::Color::Red, 390, 200, 5);
		boxQuit = drawShapes.drawRectangleOutline(window, sf::Vector2f(500, 150), sf::Color::Red, 390, 400, 5);
		drawShapes.drawText(window, 640, 200, 104, "play", sf::Color::Red, resourcesHolder);
		drawShapes.drawText(window, 640, 400, 104, "quit", sf::Color::Red, resourcesHolder);
	}
	void drawGameOver(sf::RenderWindow& window, drawSprites& drawSprites, drawShapes& drawShapes, Resources& resourcesHolder, Player& player)
	{
		drawShapes.drawText(window, 640, 20, 114, "game over", sf::Color::Red, resourcesHolder);
		drawShapes.drawText(window, 640, 150, 104, "your score", sf::Color::Red, resourcesHolder);
		drawShapes.drawText(window, 640, 280, 94, std::to_string(player.score), sf::Color::Red, resourcesHolder);
		boxOk = drawShapes.drawRectangleOutline(window, sf::Vector2f(500, 150), sf::Color::Red, 390, 430, 5);
		drawShapes.drawText(window, 640, 430, 104, "ok", sf::Color::Red, resourcesHolder);
		
	}
};


class Engine
{
public:
	void drawWindow(sf::RenderWindow& window, sf::VideoMode videoMode, std::string name)
	{
		window.close();
		window.create(videoMode, name, sf::Style::Close);
		window.setKeyRepeatEnabled(false);
		window.setFramerateLimit(60);
		//window.setVerticalSyncEnabled(true);
	}
	void playSound(Resources& resourcesHolder)
	{
		resourcesHolder.sound.play();
	}
	void eventsHandler(sf::Event& event, sf::RenderWindow& window, drawShapes& drawShapesObj, Player& player, Scenes& scenes, Resources& resourcesHolder)
	{
		if ((event.type == sf::Event::Closed) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
		{
			window.close();
		}
		if (player.gameOver == true)
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				player.gameOver = false;
				scenes.menu = true;
				resourcesHolder.loadMenuMusic();
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				if (scenes.boxOk.contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					player.gameOver = false;
					scenes.menu = true;
					resourcesHolder.loadMenuMusic();
				}
			}
		}
		else if (scenes.menu == true)
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				player.startGame();
				scenes.menu = false;
				resourcesHolder.loadGameMusic();
				
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				if (scenes.boxStart.contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					player.startGame();
					scenes.menu = false;
					resourcesHolder.loadGameMusic();
				}
				else if (scenes.boxQuit.contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					window.close();
				}
			}
		
		}
		else
		{

			if ((event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)))
			{
				player.play(0);
				playSound(resourcesHolder);


			}
			if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D))
			{
				player.play(1);
				playSound(resourcesHolder);
			}
		}
		
		/*
		if (event.type == sf::Event::Resized)
		{s
			sf::Vector2u size = window.getSize();
			printf("x: %f\n, y: %f\n", size.x, size.y);
			float hRatio = 720.f / 1280.f;
			float wRatio = 1280.f / 720.f;
			if (size.y * wRatio <= size.x)
				size.x = size.y * wRatio;
			else if (size.x * hRatio <= size.y)
				size.y = size.x * hRatio;
			window.setSize(size);
		}
		*/
	}
	/*
	void realTimeInputs(Player& player)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			player.playerPosition = 0;
			puts("XD1");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			player.playerPosition = 1;
			puts("XD2");
		}
	}
	*/

	
};




int main()
{
	FreeConsole();
	sf::RenderWindow mainWindow;
	drawShapes drawShapesObj;
	Engine engine;
	drawSprites drawSprites;
	Scenes scenes;
	Player player;
	Resources resourcesHolder;
	sf::Event event;
	sf::SoundBuffer buffer;
	srand(time(NULL));
	resourcesHolder.loadResources();
	engine.drawWindow(mainWindow, sf::VideoMode(1280, 720, 32), "narodowy drwal");
	//mainWindow.clear(sf::Color::Magenta);
	scenes.drawGame(mainWindow, drawSprites, player, drawShapesObj, resourcesHolder);
	mainWindow.display();


	while(mainWindow.isOpen())
	{
		mainWindow.clear();
		//drawSprites.drawSprite(mainWindow, "bg.png", 0, x0);
		scenes.drawGame(mainWindow, drawSprites, player, drawShapesObj, resourcesHolder);
		//engine.realTimeInputs(player);
		mainWindow.display();
		while (mainWindow.pollEvent(event))
		{
			//drawShapesObj.drawRectangle(mainWindow, sf::Vector2f(100, 640), sf::Color::Blue, 525, 0);
			engine.eventsHandler(event, mainWindow, drawShapesObj, player, scenes, resourcesHolder);
			
		}
	}
	return 0;
}
