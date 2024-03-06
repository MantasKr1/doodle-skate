#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <ctime>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "Objects.h"
#include "Utils.h"

using namespace sf;
using namespace std;

fstream file;

void UpdatePositions(Player* player, Plate* plate, vector<Ground*>& grounds, Bullet* bullet, Enemy* enemy, float& dy, float& score, bool& gameEnd, bool& jump, bool& doublejump, clock_t& now, float& dx, float& gap, bool& shoot)
{
    dx += 0.005f;

    score += (dx / 100);

    gap += (dx / 250);

    plate->x -= dx;

    dy += 0.2f;

    enemy->x -= dx;

    // Generating

    // Plates
    if (plate->x < 0 - PLATES_WIDTH)
    {
        plate->x = WINDOW_WIDTH + 10;
        plate->y = 100 + float(rand() % ((WINDOW_HEIGHT - GROUND_HEIGHT) - 100));
    }

    // Grounds
    for (int i = 0; i < GROUND_AMOUNT; ++i)
    {
        grounds[i]->x -= dx;

        if ((grounds[0]->x + GROUND_WIDTH) < 0)
        {
            for (int i = 0; i < GROUND_AMOUNT; ++i)
            {
                if (i == GROUND_AMOUNT - 1)
                {
                    grounds[i]->x = (grounds[i]->x + GROUND_WIDTH + gap) + float(rand() % ((WINDOW_WIDTH - 2 * GROUND_WIDTH)));
                    grounds[i]->kuris = grounds[0]->kuris;
                }
                else
                {
                    grounds[i]->x = grounds[i + 1]->x;
                    grounds[i]->kuris = grounds[i + 1]->kuris;
                }
            }
        }
    }

    // Enemy
    if (enemy->x + ENEMY_WIDTH < 0 || utils::IsShot(*enemy, *bullet))
    {
        enemy->x = grounds[3]->x + (GROUND_WIDTH / 3);
        enemy->y = (WINDOW_HEIGHT - GROUND_HEIGHT - 20);
    }

    if (bullet->x > WINDOW_WIDTH) bullet->x = -50;

    // Shoot
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !shoot)
    {
        shoot = 1;
        bullet->x = player->x + 60;
        bullet->y = player->y + 30;
    }

    if (shoot)
    {
        bullet->x += 7 * dx;
    }

    if (bullet->x > WINDOW_WIDTH) shoot = 0;

    // Jumping mechanics
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
    {
        if (jump)
        {
            dy = PLAYER_JUMP_V;
            now = clock();
            jump = 0;
            doublejump = 1;
        }
        else if (doublejump && !(clock() - now < (0.23 * CLOCKS_PER_SEC)))
        {
            dy = PLAYER_JUMP_V;
            doublejump = 0;
        }
    }

    // Game END
    if (player->y > WINDOW_HEIGHT)
    {
        gameEnd = 1;
    }

    if (utils::IsHit(*player, *enemy))
    {
        gameEnd = 1;
    }

    // Check if on the ground
    for (int i = 0; i < GROUND_AMOUNT; ++i)
    {
        if (utils::IsOnGround(*player, *grounds[i]) && dy > 0)
        {
            player->y = WINDOW_HEIGHT - GROUND_HEIGHT;

            dy = 0;
            jump = 1;
            doublejump = 0;
        }
    }

    // Check if on plate
    if ((utils::InOnPlate(*player, *plate)) && dy > 0)
    {
        jump = 1;
    }

    player->y += dy;
}

void updatehighScore(float score, int& best)
{
    file.open("highscore.txt", ofstream::out | ofstream::trunc);
    best = static_cast<int>(score);
    file << static_cast<int>(score);
    file.close();
}

int main()
{
    bool firstTime = 1;

start:

    bool gameEnd = 0;

    srand(static_cast<unsigned>(time(nullptr)));
    RenderWindow app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Doodle");
    app.setFramerateLimit(60);

    Texture tBackground, tPlayer, tPlatform, tGround1, tGround2, tGround3, tEnemy;
    tBackground.loadFromFile("resources/background.png");
    tPlayer.loadFromFile("resources/doodle2.png");
    tPlatform.loadFromFile("resources/platform.png");
    tGround1.loadFromFile("resources/platform1.png");
    tGround2.loadFromFile("resources/platform2.png");
    tGround3.loadFromFile("resources/platform3.png");
    tEnemy.loadFromFile("resources/Enemy.png");

    sf::Font font;
    font.loadFromFile("resources/arialbd.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString("0");
    text.setCharacterSize(50);
    text.setFillColor(Color::White);
    text.setOutlineThickness(5);
    text.setOutlineColor(Color::Black);
    text.setPosition(0, 0);

    sf::Text menu;
    menu.setFont(font);
    menu.setString("0");
    menu.setCharacterSize(100);
    menu.setFillColor(Color::White);
    menu.setOutlineThickness(5);
    menu.setOutlineColor(Color::Black);
    menu.setPosition(220, 220);

    Sprite sprBackground(tBackground);
    Sprite sprPlayer(tPlayer);
    Sprite sprPlatform(tPlatform);
    Sprite sprGround1(tGround1);
    Sprite sprGround2(tGround2);
    Sprite sprGround3(tGround3);
    Sprite sprEnemy(tEnemy);

    sf::CircleShape shapeBullet(5);
    shapeBullet.setFillColor(sf::Color::Black);
    shapeBullet.setOutlineThickness(2.5);
    shapeBullet.setOutlineColor(sf::Color::Red);

#ifdef SOUND_ON
    sf::SoundBuffer buffer;
    buffer.loadFromFile("resources/music.wav");

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();
    sound.setLoop(true);
#endif // SOUND_ON

    // Player
    Player* player = new Player();
    player->x = WINDOW_WIDTH / 10;
    player->y = WINDOW_HEIGHT - GROUND_HEIGHT - 200;

    // Grounds
    vector<Ground*> grounds;
    for (int i = 0; i < GROUND_AMOUNT; ++i)
    {
        if (i == 0)
        {
            grounds.push_back(new Ground());
            grounds[i]->x = 0;
            grounds[i]->y = (WINDOW_HEIGHT - GROUND_HEIGHT);
        }
        else
        {
            grounds.push_back(new Ground());
            grounds[i]->x = (50 + grounds[i - 1]->x + GROUND_WIDTH) + float(rand() % ((WINDOW_WIDTH - 2 * GROUND_WIDTH) - 50));
            grounds[i]->y = (WINDOW_HEIGHT - GROUND_HEIGHT);
        }
    }
    grounds[0]->kuris = 1;
    grounds[1]->kuris = 2;
    grounds[2]->kuris = 3;
    grounds[3]->kuris = 1;

    // Plate
    Plate* plate = new Plate();
    plate->x = WINDOW_WIDTH + 10;
    plate->y = 100 + float(rand() % ((WINDOW_HEIGHT - GROUND_HEIGHT)) - 100);

    // Enemy
    Enemy* enemy = new Enemy();
    enemy->x = grounds[3]->x + (GROUND_WIDTH / 3);
    enemy->y = (WINDOW_HEIGHT - GROUND_HEIGHT - 20);

    // Bullet
    Bullet* bullet = new Bullet();
    bullet->x = player->x;
    bullet->y = player->y;

    // Initial data
    float dx = 3.5;
    float dy = 0;
    float score = 0;
    float gap = 50;
    bool jump = 0;
    bool doublejump = 0;
    bool shoot = 0;

    clock_t now = clock();

    // Highscore
    int best;
    file.open("highscore.txt");
    file >> best;

    // App
    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

        // Draw Menu screen
        if (firstTime)
        {
        loop:

            if (e.type == Event::Closed)
            {
                app.close();
            }

            sprBackground.setPosition(0, 0);
            app.draw(sprBackground);

            menu.setString("Doodle Skate");
            app.draw(menu);

            sprPlayer.setPosition(player->x, player->y);
            app.draw(sprPlayer);

            app.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                firstTime = 0;
                goto start;
            }

            goto loop;
        }

        // Update
        if (!gameEnd) UpdatePositions(player, plate, grounds, bullet, enemy, dy, score, gameEnd, jump, doublejump, now, dx, gap, shoot);

        if (score > best)
        {
            updatehighScore(score, best);
        }

        sprBackground.setPosition(0, 0);
        app.draw(sprBackground);

        // Draw grounds
        for (int i = 0; i < GROUND_AMOUNT; ++i)
        {
            switch (grounds[i]->kuris)
            {
            case 1:
                sprGround1.setPosition(grounds[i]->x, grounds[i]->y);
                app.draw(sprGround1);
                break;
            case 2:
                sprGround2.setPosition(grounds[i]->x, grounds[i]->y);
                app.draw(sprGround2);
                break;
            case 3:
                sprGround3.setPosition(grounds[i]->x, grounds[i]->y);
                app.draw(sprGround3);
                break;
            }
        }

        // Draw player
        sprPlayer.setPosition(player->x, player->y);
        app.draw(sprPlayer);

        // Draw plate
        sprPlatform.setPosition(plate->x, plate->y);
        app.draw(sprPlatform);

        // Draw bullet
        if (shoot)
        {
            shapeBullet.setPosition(bullet->x, bullet->y);
            app.draw(shapeBullet);
        }

        // Draw enemy
        sprEnemy.setPosition(enemy->x, enemy->y);
        app.draw(sprEnemy);

        if (gameEnd)
        {
            if (static_cast<int>(score) < best)
            {
                text.setFillColor(Color::Red);
            }
        }

        text.setString("Score: " + to_string(static_cast<int>(score)) + "   Best: " + to_string(best));

        if (static_cast<int>(score) == best) text.setString("Score: " + to_string(static_cast<int>(score)) + " - Best score");

        app.draw(text);

        if (gameEnd)
        {
            menu.setPosition(WINDOW_WIDTH / 4 + 30, 250);
            menu.setString("You died!");

            menu.setFillColor(Color::Red);
            app.draw(menu);
        }

        app.display();

        if (gameEnd)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                double delay = 0.5;
                delay *= CLOCKS_PER_SEC;
                now = clock();
                while (clock() - now < delay);
                app.close();
                goto start;
            }
        }
    }

    // Clean up dynamic memory
    delete player;
    delete plate;
    delete bullet;
    delete enemy;

    for (auto ground : grounds)
    {
        delete ground;
    }

    return 0;
}
