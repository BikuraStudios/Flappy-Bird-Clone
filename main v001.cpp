#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

struct ParallaxLayer
{
    sf::Sprite sprite;
    float offset = 0.0f;
    float speed = 0.0f;
    float resetThreshhold = 3072.0f;

    ParallaxLayer(const sf::Texture& texture, float speed, float resetThreshhold = 3072.0f)
        : sprite(texture), speed(speed), resetThreshhold(resetThreshhold)
    {
    }

    void update(float deltaTime)
    {
        if (offset > -resetThreshhold)
            offset -= speed * deltaTime;
        if (offset <= -resetThreshhold)
            offset = 0.0f;

        sprite.setPosition(sf::Vector2(offset, 0.0f));
    }
    void draw(sf::RenderWindow& window)
    {
        window.draw(sprite);
    }
};

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({ 1536u, 1024u }), "Flying Robot");
    window.setFramerateLimit(144);

    sf::View gameView(sf::FloatRect({ 0.0f, 0.0f }, { 1536.0f, 1024.0f }));

    sf::SoundBuffer buffer_jump("jump.wav");
    sf::Sound sound_jump(buffer_jump);

    sf::Texture texture_clouds("clouds.png");
    sf::Sprite sprite_clouds(texture_clouds);
    ParallaxLayer clouds(texture_clouds, 15.0f);

    sf::Texture texture_cloudsTwo("cloudsTwo.png");
    sf::Sprite sprite_cloudsTwo(texture_cloudsTwo);
    ParallaxLayer cloudsTwo(texture_cloudsTwo, 5.0f);

    sf::Texture texture_hills("hills.png");
    sf::Sprite sprite_hills(texture_hills);
    ParallaxLayer hills(texture_hills, 20.0f);

    sf::Texture texture_teaRows("teaRows.png");
    sf::Sprite sprite_teaRows(texture_teaRows);
    ParallaxLayer teaRows(texture_teaRows, 60.0f);

    sf::Texture texture_groundBar("groundBar.png");
    sf::Sprite sprite_groundBar(texture_groundBar);
    ParallaxLayer groundBar(texture_groundBar, 100.0f);

    sf::Texture texture_robotDefault("robotDefault.png");
    sf::Sprite sprite_robotDefault(texture_robotDefault);

    sf::Texture texture_robotJump("robotJump.png");
    sf::Sprite sprite_robotJump(texture_robotJump);

    sf::Clock clock;

    auto robotPosition{ sf::Vector2f(300.f, 700.f) };
    auto robotSpeed{ 300.f };
    auto robotVelocity{ 0.f };

    while (window.isOpen())
    {

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::Resized>())
            {
                sf::FloatRect viewport;
                sf::Vector2u size = window.getSize();
                auto [windowWidth, windowHeight] = size;
                float aspectRatio = windowWidth / windowHeight;
                float targetRatio{ 1536.f / 1024.f };


                if (aspectRatio > targetRatio)
                {
                    // Window is too wide — horizontal letterboxing
                    float width = targetRatio / aspectRatio;
                    viewport = sf::FloatRect({ ((1.f - width) / 2.f), 0.f }, { width, 1.f });
                }
                else
                {
                    // Window is too tall — vertical letterboxing
                    float height = aspectRatio / targetRatio;
                    viewport = sf::FloatRect({ 0.f, ((1.f - height) / 2.f) }, { 1.f, height });
                }

                gameView.setViewport(viewport);
            }
        }
        float deltaTime = clock.restart().asSeconds();

        clouds.update(deltaTime);
        cloudsTwo.update(deltaTime);
        hills.update(deltaTime);
        teaRows.update(deltaTime);
        groundBar.update(deltaTime);



        // robot function
        robotVelocity = robotSpeed * deltaTime;



        window.setView(gameView);
        window.clear();
        clouds.draw(window);
        cloudsTwo.draw(window);
        hills.draw(window);
        teaRows.draw(window);
        groundBar.draw(window);

        sprite_robotDefault.setPosition(robotPosition);
        sprite_robotJump.setPosition(robotPosition);

        //Jumping check
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            robotPosition.y -= robotVelocity;
            window.draw(sprite_robotJump);
            sound_jump.play();

        }
        else
        {
            robotPosition.y += (1.25f * robotVelocity);
            window.draw(sprite_robotDefault);
        }


        window.display();
    }

}
