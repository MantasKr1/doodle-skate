#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Objects.h"
#include "Utils.h"

using namespace sf;
using namespace std;

void UpdatePositions(Player &player, Plate plates[], float &dy, float &score)
{
	const float dx = 3.5f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		player.x -= dx;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player.x += dx;

	if (player.x + PLAYER_WIDTH / 2 > WINDOW_WIDTH)
		player.x = -PLAYER_WIDTH / 2;

	if (player.x + PLAYER_WIDTH / 2 < 0)
		player.x = WINDOW_WIDTH - PLAYER_WIDTH / 2;

	dy += 0.2f; // gravitacija su pagreiciu

	player.y += dy;

	if (player.y > WINDOW_HEIGHT)
		dy = PLAYER_JUMP_V;

	if (player.y < MAX_PLAYER_Y)
	{
		player.y = MAX_PLAYER_Y;
		score -= 0.05f * dy;

		for (int i = 0; i < PLATES_AMOUNT; ++i)
		{
			plates[i].y -= dy;

			if (plates[i].y > WINDOW_HEIGHT)
			{
				plates[i].y = float(rand() % 10);
				plates[i].x = float(rand() % (WINDOW_WIDTH - PLATES_WIDTH));
			}
		}
	}

	for (int i = 0; i < PLATES_AMOUNT; ++i)
	{
		if (utils::InOnPlate(player, plates[i]) && dy > 0)
			dy = PLAYER_JUMP_V;
	}
}

int main()
{
	srand((unsigned)time(nullptr));
	RenderWindow app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Doodle Jump");
	app.setFramerateLimit(60);

	Texture tBackground, tPlayer1, tPlatform, tPlayer2;
	tBackground.loadFromFile("resources/background.png");
	tPlayer1.loadFromFile("resources/doodle1.png");
	tPlayer2.loadFromFile("resources/doodle2.png");
	tPlatform.loadFromFile("resources/platform.png");

	sf::Font font;
	font.loadFromFile("resources/arialbd.ttf");

	sf::Text text;
	text.setFont(font);
	text.setString("0");
	text.setCharacterSize(40);
	text.setFillColor(Color::Red);
	text.setOutlineThickness(1);
	text.setOutlineColor(Color::Black);
	text.setPosition(WINDOW_WIDTH / 2.0f - 25.f, 10.f);

	Sprite sprBackground(tBackground);
	Sprite sprPlayer(tPlayer1);
	Sprite sprPlatform(tPlatform);

#ifdef SOUND_ON
	sf::SoundBuffer buffer;
	buffer.loadFromFile("resources/music.wav");

	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();
	sound.setLoop(true);
#endif // SOUND_ON

	Player player;
	player.x = WINDOW_WIDTH / 2;
	player.y = MAX_PLAYER_Y;

	Plate plates[PLATES_AMOUNT];

	for (int i = 0; i < PLATES_AMOUNT; ++i)
	{
		plates[i].x = float(rand() % (WINDOW_WIDTH - PLATES_WIDTH));
		plates[i].y = (float)WINDOW_HEIGHT / PLATES_AMOUNT * i;
	}

	float dy = 0;
	float score = 0;

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
			sprPlayer.setTexture(tPlayer1);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		{
			sprPlayer.setTexture(tPlayer2);
			dy -= 0.21f;
		}

		UpdatePositions(player, plates, dy, score);

		app.draw(sprBackground);

		for (int i = 0; i < PLATES_AMOUNT; ++i)
		{
			sprPlatform.setPosition(plates[i].x, plates[i].y);
			app.draw(sprPlatform);
		}

		sprPlayer.setPosition(player.x, player.y);
		app.draw(sprPlayer);

		text.setString(to_string((int)score));
		app.draw(text);

		app.display();
	}

	return 0;
}
