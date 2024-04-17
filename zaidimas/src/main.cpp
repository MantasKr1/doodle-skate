#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <cstdlib>

#include <vector>
#include <map>

#include "Objects.h"
#include "Utils.h"

using namespace sf;
using namespace std;

fstream file;

Sprite randGround(Sprites sprite)
{
    Sprite temp;
    int randomas = rand() % 3 + 1;
    if (randomas == 1) temp = sprite.Ground1();
    else if (randomas == 2) temp = sprite.Ground2();
    else if (randomas == 3) temp = sprite.Ground3();

    return temp;
}

void UpdatePositions(Player& player, Plate& plate, vector<Ground>& grounds, vector<Bullet>& bullets, Enemy& enemy,
    float& dy, float& score, bool& gameEnd, clock_t& now, float& dx, float& gap, Sprites sprite)
{
    dx += 0.005f;

    score += (dx / 100);

    gap += (dx / 250);

    

    dy += 0.2f;

    // Plates

    plate.ChangeX(-dx);

    if (plate.X() < 0-PLATES_WIDTH)
    {
        plate.SetPosX(grounds.back().X() - 50);
        plate.SetPosY(player.Y()+PLAYER_WIDTH);
    }

    // Check if on plate
    if ((utils::InOnPlate(player, plate)) && dy > 0)
    {
        player.Jump();
    }

    // Grounds
    if ((grounds.front().X() + GROUND_WIDTH) < 0)
    {
        grounds.erase(grounds.begin());
        float randomGap = float(rand() % ((WINDOW_WIDTH - 2 * GROUND_WIDTH) - 50));
        if (randomGap < 100) randomGap = 0;
        grounds.push_back(Ground((grounds.back().X() + GROUND_WIDTH) + randomGap,
        (WINDOW_HEIGHT - GROUND_HEIGHT), randGround(sprite)));
    }

    for (auto &ground : grounds)
    {
        ground.ChangeX(-dx);
    }

    // Check if on the ground

    for (auto ground : grounds)
    {
        if (utils::IsOnGround(player, ground) && dy > 0)
        {
            dy = 0;
            player.PosY(ground.TopY());
            player.Jump();
        }
    }

    // Enemy

    enemy.ChangeX(-dx);

    if (enemy.X() + ENEMY_WIDTH < 0)
    {
        enemy.ChangeX(grounds.back().EnemyPos());
    }

    // Bullets

    //Erase bullets
    if (!bullets.empty())
    {
        for (vector <Bullet> :: iterator bullet = bullets.begin(); bullet != bullets.end(); )
        {
            bullet->ChangeX(7 * dx);

            if (utils::IsShot(enemy, *bullet))
            {
                enemy.ChangeX(grounds.back().EnemyPos());
                bullet = bullets.erase(bullet);
            }
            else if (bullet->X() > WINDOW_WIDTH)
            {
                bullet = bullets.erase(bullet);
            }
            else
            {
                bool hit = 0;
                for (auto ground : grounds)
                {
                    if (utils::HitGround(ground, *bullet)) hit = true;
                }
                if (hit)
                {
                    bullet = bullets.erase(bullet);
                }
                else bullet++;
            }
        }
    }

    // Bullet Shoot
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if(bullets.empty()) bullets.push_back(Bullet(player.X() + 60, player.Y() + 30, clock()));
        else if (!(clock() - bullets.back().Time() < (0.1 * CLOCKS_PER_SEC)))bullets.push_back(Bullet(player.X() + 60, player.Y() + 30, clock()));
    }

    // Player move

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player.X() > 0)
    {
        player.ChangeX((-2 * dx)/2);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player.X() < WINDOW_WIDTH-PLAYER_WIDTH)
    {
        player.ChangeX(dx/2);
    }

    // Player jump

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if (player.canJump())
        {
            dy = PLAYER_JUMP_V;
            now = clock();
            player.Jumped();
        }
        else if (player.canDoubleJump() && !(clock() - now < (0.23 * CLOCKS_PER_SEC)))
        {
            dy = PLAYER_JUMP_V;
            player.DoubleJumped();
        }
    }  

    // Game END
    if (player.Y() > WINDOW_HEIGHT)
    {
        gameEnd = 1;
    }

    if (utils::IsHit(player, enemy))
    {
        gameEnd = 1;
    }

    player.ChangeY(dy);
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

    Sprites sprite;

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
    Player player(WINDOW_WIDTH / 10, WINDOW_HEIGHT - GROUND_HEIGHT - 200);

    // Grounds

    vector <Ground> grounds;

    for (int i = 0; i < GROUND_AMOUNT; i++)
    {
        if (i == 0)
        {
            grounds.push_back(Ground(0, (WINDOW_HEIGHT - GROUND_HEIGHT), randGround(sprite)));
        }
        else
        {
            grounds.push_back(Ground((50 + grounds.back().X() + GROUND_WIDTH) + float(rand() % ((WINDOW_WIDTH - 2 * GROUND_WIDTH) - 50)), 
            (WINDOW_HEIGHT - GROUND_HEIGHT), randGround(sprite)));
            
        }
    }

    // Plate
    Plate plate (grounds.back().X(), player.Y());

    // Enemy
    Enemy enemy (grounds.back().EnemyPos(), (WINDOW_HEIGHT - GROUND_HEIGHT - 20));

    // Bullet
    vector <Bullet> bullets;

    // Initial data
    float dx = 3.5;
    float dy = 0;
    float score = 0;
    float gap = 50;

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

            sprite.Background().setPosition(0, 0);
            app.draw(sprite.Background());

            menu.setString("Doodle Skate");
            app.draw(menu);

            sprite.Player().setPosition(player.X(), player.Y());
            app.draw(sprite.Player());

            app.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                firstTime = 0;
                goto start;
            }

            goto loop;
        }

        // Update
        if (!gameEnd) UpdatePositions(player, plate, grounds, bullets, enemy, dy, score, gameEnd, now, dx, gap, sprite);

        if (score > best)
        {
            updatehighScore(score, best);
        }

        sprite.Background().setPosition(0, 0);
        app.draw(sprite.Background());

        // Draw grounds

        for (auto ground : grounds)
        {
            ground.Sprite().setPosition(ground.X(), ground.Y());
            app.draw(ground.Sprite());
        }

        // Draw player

        sprite.Player().setPosition(player.X(), player.Y());
        app.draw(sprite.Player());

        // Draw plate
        
        sprite.Plate().setPosition(plate.X(), plate.Y());
        app.draw(sprite.Plate());

        // Draw bullet

        for (auto bullet : bullets)
        {
            shapeBullet.setPosition(bullet.X(), bullet.Y());
            app.draw(shapeBullet);
        }

        // Draw enemy

        sprite.Enemy().setPosition(enemy.X(), enemy.Y());
        app.draw(sprite.Enemy());

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

    return 0;
}
