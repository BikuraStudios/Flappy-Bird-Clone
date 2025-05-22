#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <iostream>



int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({ 1536u, 1024u }), "Flying Robot");
    window.setFramerateLimit(144);

    sf::View gameView(sf::FloatRect({ 0.0f, 0.0f }, { 1536.0f, 1024.0f }));

    sf::Texture texture_clouds("clouds.png");
    sf::Sprite sprite_clouds(texture_clouds);

    sf::Texture texture_cloudsTwo("cloudsTwo.png");
    sf::Sprite sprite_cloudsTwo(texture_cloudsTwo);

    sf::Texture texture_hills("hills.png");
    sf::Sprite sprite_hills(texture_hills);

    sf::Texture texture_groundBar("groundBar.png");
    sf::Sprite sprite_groundBar(texture_groundBar);

    sf::Texture texture_teaRows("teaRows.png");
    sf::Sprite sprite_teaRows(texture_teaRows);


    sf::Texture texture_robotDefault("robotDefault.png");
    sf::Sprite sprite_robotDefault(texture_robotDefault);

    sf::Texture texture_robotJump("robotJump.png");
    sf::Sprite sprite_robotJump(texture_robotJump);

    sf::Clock clock;

    // find a way to get all offsets done by a function
    float cloudOffSet{ 0.0f };
    float cloudSpeed(1.0f);

    float cloudTwoOffSet{ 0.0f };
    float cloudTwoSpeed(1.0f);

    float hillOffSet{ 0.0f };
    float hillSpeed(10.0f);

    float teaRowOffSet{ 0.0f };
    float teaRowSpeed(60.0f);

    float groundBarOffSet{ 0.0f };
    float groundBarSpeed(100.0f);

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

        
        // make this a cloud function
        cloudOffSet -= cloudSpeed * deltaTime;
        
        if (cloudOffSet > -3072.0f)
        {
            cloudOffSet -= 0.50f;
            std::cout << cloudOffSet;
        }
        
        if (cloudOffSet <= -3072.0f)
            cloudOffSet = 0;

        cloudTwoOffSet -= cloudTwoSpeed * deltaTime;

        if (cloudTwoOffSet > -3072.0f)
        {
            cloudTwoOffSet -= 0.750f;
            std::cout << cloudTwoOffSet;
        }

        if (cloudTwoOffSet <= -3072.0f)
            cloudTwoOffSet = 0;

        //end cloud function

        // hill function
        hillOffSet -= hillSpeed * deltaTime;

        if (hillOffSet > -3072.0f)
        {
            hillOffSet -= 1.0f;
            std::cout << hillOffSet;
        }

        if (hillOffSet <= -3072.0f)
            hillOffSet = 0;
        // end hill function
         
        
        // tea function
        teaRowOffSet -= teaRowSpeed * deltaTime;

        if (teaRowOffSet > -3072.0f)
        {
            teaRowOffSet -= 1.0f;
            std::cout << teaRowOffSet;
        }

        if (teaRowOffSet <= -3072.0f)
            teaRowOffSet = 0;
        //end tea function
        
        
        //ground function
        groundBarOffSet -= groundBarSpeed * deltaTime;

        if (groundBarOffSet > -3072.0f)
        {
            groundBarOffSet -= 1.0f;
            std::cout << groundBarOffSet;
        }

        if (groundBarOffSet <= -3072.0f)
            groundBarOffSet = 0;
        // end ground function
        
        // robot function
        robotVelocity = robotSpeed * deltaTime;
        

        sprite_clouds.setPosition(sf::Vector2f(cloudOffSet, 0));
        sprite_cloudsTwo.setPosition(sf::Vector2f(cloudTwoOffSet, 0));
        sprite_hills.setPosition(sf::Vector2f(hillOffSet, 0));
        sprite_teaRows.setPosition(sf::Vector2f(teaRowOffSet, 0));
        sprite_groundBar.setPosition(sf::Vector2f(groundBarOffSet, 0));
        sprite_robotDefault.setPosition(sf::Vector2f(robotPosition));
        sprite_robotJump.setPosition(sf::Vector2f(robotPosition));

        window.setView(gameView);
        window.clear();
        window.draw(sprite_clouds);
        window.draw(sprite_cloudsTwo);
       
        window.draw(sprite_hills);
        window.draw(sprite_teaRows);
        window.draw(sprite_groundBar);
        //Jumping check
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            robotPosition.y -= robotVelocity;
            window.draw(sprite_robotJump);
        
        }
        else
        {
            robotPosition.y += (1.25f * robotVelocity);
            window.draw(sprite_robotDefault);
        }


        window.display();
    }
}
