#include <SFML/Graphics.hpp>
#include <iostream>



int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1536u, 1024u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    sf::Texture texture_clouds("clouds.png");
    sf::Sprite sprite_clouds(texture_clouds);

    sf::Texture texture_hills("hills.png");
    sf::Sprite sprite_hills(texture_hills);

    sf::Texture texture_groundBar("groundBar.png");
    sf::Sprite sprite_groundBar(texture_groundBar);

    sf::Texture texture_teaRows("teaRows.png");
    sf::Sprite sprite_teaRows(texture_teaRows);

    sf::Clock clock;

    // find a way to get all offsets done by a function
    float cloudOffSet{ 0.0f };
    float cloudSpeed(25.0f);

    float hillOffSet{ 0.0f };
    float hillSpeed(10.0f);

    float teaRowOffSet{ 0.0f };
    float teaRowSpeed(60.0f);

    float groundBarOffSet{ 0.0f };
    float groundBarSpeed(100.0f);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        
        float deltaTime = clock.restart().asSeconds();

        
        // make this a cloud function
        cloudOffSet -= cloudSpeed * deltaTime;
        
        if (cloudOffSet > -3072.0f)
        {
            cloudOffSet -= 1.0f;
            std::cout << cloudOffSet;
        }
        
        if (cloudOffSet <= -3072.0f)
            cloudOffSet = 0;
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
            

        sprite_clouds.setPosition(sf::Vector2f(cloudOffSet, 0));
        sprite_hills.setPosition(sf::Vector2f(hillOffSet, 0));
        sprite_teaRows.setPosition(sf::Vector2f(teaRowOffSet, 0));
        sprite_groundBar.setPosition(sf::Vector2f(groundBarOffSet, 0));

        window.clear();
        window.draw(sprite_clouds);
        window.draw(sprite_hills);
        window.draw(sprite_teaRows);
        window.draw(sprite_groundBar);
        window.display();
    }
}
