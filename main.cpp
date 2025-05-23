#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>

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

struct PipePair
{
    sf::Sprite topPipe;
    sf::Sprite bottomPipe;
    float x;     // Current x position
    float speed; // Horizontal scroll speed

    PipePair(const sf::Texture& topTexture, const sf::Texture& bottomTexture, float startX, float gapY, float speed)
        : topPipe(topTexture), bottomPipe(bottomTexture), x(startX), speed(speed)
    {
        topPipe.setTexture(topTexture);
        bottomPipe.setTexture(bottomTexture);

        topPipe.setPosition(sf::Vector2f{ x, gapY - 1520.0f });
        bottomPipe.setPosition(sf::Vector2f{ x, gapY });
    }

    void update(float deltaTime)
    {
        x -= speed * deltaTime;

        topPipe.setPosition(sf::Vector2f{ x, topPipe.getPosition().y });
        bottomPipe.setPosition(sf::Vector2f{ x, bottomPipe.getPosition().y });
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(topPipe);
        window.draw(bottomPipe);
    }
};

float pipeRandom()
{
return static_cast<float>(rand() % 470 + 420);
}


int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({ 1280u, 720u }), "Flying Robot");
    window.setFramerateLimit(144);
    window.setPosition({ 300,150 });
    window.requestFocus();
    
    std::srand(static_cast<unsigned>(std::time(nullptr)));

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
    auto textureSize = texture_robotDefault.getSize();
    sf::Vector2f robotSize{ static_cast<float>(textureSize.x), static_cast<float>(textureSize.y) };

    sf::Texture texture_pipeB("pipeB.png");
    sf::Sprite sprite_pipeB(texture_pipeB);

    sf::Texture texture_pipeT("pipeT.png");
    sf::Sprite sprite_pipeT(texture_pipeT);

    sf::FloatRect robotBounds = sprite_robotDefault.getGlobalBounds();
    

    std::vector<PipePair> pipes;
    float pipeSpawnTimer = 0.0f;
    const float pipeSpacing = 343.0f;

    sf::Texture texture_robotJump("robotJump.png");
    sf::Sprite sprite_robotJump(texture_robotJump);

    sf::Clock clock;

    auto robotPosition{ sf::Vector2f(300.f, 700.f) };
    auto robotSpeed{ 300.f };
    auto robotVelocity{ 0.f };

    bool wasJumping = false;
    const float gravity = 500.0f;
    const float jumpImpulse = -300.0f;
    const float groundY = 994.0f;
    bool wasJumpPressed = false;
    bool gameOver = false;

    while (window.isOpen())
    {

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
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

        if (!gameOver)
        {
        clouds.update(deltaTime);
        cloudsTwo.update(deltaTime);
        hills.update(deltaTime);
        teaRows.update(deltaTime);
        // All pipe stuff goes here so it renders behind groundbar

        const float pipeSpeed = 200.0f;
         pipeSpawnTimer += 0.70*deltaTime;
        if (pipeSpawnTimer >= pipeSpacing / pipeSpeed)
        {
            float gapY = pipeRandom();
            pipes.emplace_back(texture_pipeT, texture_pipeB, 1536.0f, gapY, pipeSpeed);
            pipeSpawnTimer = 0.0f;
            
        }
        
        //Update Position
        robotPosition.y += robotVelocity * deltaTime;
        sprite_robotDefault.setPosition(robotPosition);
        sprite_robotJump.setPosition(robotPosition);
        sf::FloatRect robotBounds = sprite_robotDefault.getGlobalBounds();

        for (auto& pipe : pipes)
        {
            if (robotBounds.findIntersection(pipe.topPipe.getGlobalBounds()) ||
                robotBounds.findIntersection(pipe.bottomPipe.getGlobalBounds()))
            {
                robotPosition.y = (groundY - robotSize.y);
                robotVelocity = 0.0f;
                gameOver = true;
            }
            pipe.update(deltaTime);
            

        }
        // robot function

        //Apply Gravity
        if (!gameOver)
        {
            robotVelocity += gravity * deltaTime;
        }
        //Jumping check
        bool isJumping = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);

        if (isJumping && !wasJumpPressed && !gameOver)
        {
            robotVelocity = jumpImpulse; // Instant upward push
            sound_jump.play();
        }
        wasJumpPressed = isJumping;

        if (robotPosition.y >= (groundY + 2))
        {
            robotPosition.y = (groundY - robotSize.y);
            robotVelocity = 0.0f;
            gameOver = true;
        }
        

        groundBar.update(deltaTime);
            
       
        }
        
        pipes.erase(std::remove_if(pipes.begin(), pipes.end(),
            [](const PipePair& pipe) { return pipe.x < -200.0f; }), pipes.end());
        

        window.setView(gameView);
        window.clear();



        clouds.draw(window);
        cloudsTwo.draw(window);
        hills.draw(window);
        teaRows.draw(window);
        for (auto& pipe : pipes)
        {
            pipe.draw(window);
        }
        groundBar.draw(window);

       

        

        if (robotVelocity < 0)
            window.draw(sprite_robotJump);
        else
            window.draw(sprite_robotDefault);

       
            

        
        window.display();
        
    }

}