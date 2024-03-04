#include "SFML/Graphics.hpp"
#include <iostream>
#include <algorithm>

void windowInitialization(sf::RenderWindow& window) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    constexpr int width{ 950 };
    constexpr int height{ 600 };
    window.create(sf::VideoMode(width, height), "Pong", sf::Style::Close, settings);
    window.setFramerateLimit(100);
}

sf::RectangleShape playerRectangleInitialization(sf::RenderWindow& window, int numberSideX) {
    constexpr int halfSizeRectangleX{ 9 };
    constexpr int halfSizeRectangleY{ 37 };
    sf::RectangleShape shape1;
    shape1.setSize(sf::Vector2f(halfSizeRectangleX * 2, halfSizeRectangleY * 2));
    shape1.setOrigin(halfSizeRectangleX, halfSizeRectangleY);
    shape1.setFillColor(sf::Color::White);
    shape1.setPosition(window.getSize().x - numberSideX, window.getSize().y / 2);
    return shape1;
}

sf::RectangleShape aiRectangleInitialization(sf::RectangleShape shape1, sf::RenderWindow& window, int numberSideX) {
    shape1.setPosition(numberSideX, window.getSize().y / 2);
    return shape1;
}

sf::RectangleShape middleLineInitialization(sf::RenderWindow& window) {
    constexpr int middleLineHalfSizeX{ 1 };
    const int middleLineHalfSizeY{ static_cast<int>(window.getSize().y) / 2 };
    sf::RectangleShape middleLine;
    middleLine.setSize(sf::Vector2f(middleLineHalfSizeX * 2, window.getSize().y));
    middleLine.setFillColor(sf::Color(255, 255, 255, 150));
    middleLine.setOrigin(middleLine.getLocalBounds().width / 2, middleLine.getLocalBounds().height / 2);
    middleLine.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    return middleLine;
}

sf::CircleShape ballInitialization(sf::RenderWindow& window) {
    constexpr int ballSize{ 20 };
    constexpr int ballRadius{ ballSize / 2 };
    sf::CircleShape shapeBall;
    shapeBall.setRadius(ballRadius);
    shapeBall.setOrigin(ballRadius, ballRadius);
    shapeBall.setFillColor(sf::Color::White);
    shapeBall.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    return shapeBall;
}

sf::Font defaultFont() {
    sf::Font font;
    font.loadFromFile("./Heavitas.ttf");
    return font;
}

sf::Text playerTextF(sf::Font& font, sf::RenderWindow& window) {
    constexpr int spaceBetweenBar{ 60 };
    constexpr int fontHalfSize{ 40 };
    constexpr int spaceY{ 25 };

    sf::Text playerText("0", font, fontHalfSize * 2);
    playerText.setOrigin(playerText.getLocalBounds().width / 2, playerText.getLocalBounds().height / 2);
    playerText.setPosition(window.getSize().x / 2 + spaceBetweenBar, spaceY);
    return playerText;
}

sf::Text aiTextF(sf::Font& font, sf::RenderWindow& window) {
    constexpr int spaceBetweenBar{ 60 };
    constexpr int fontHalfSize{ 40 };
    constexpr int spaceY{ 25 };
   
    sf::Text aiText("0", font, fontHalfSize * 2);
    aiText.setOrigin(aiText.getLocalBounds().width/2, aiText.getLocalBounds().height / 2);
    aiText.setPosition(window.getSize().x / 2 - spaceBetweenBar, spaceY);
    return aiText;
}

sf::Text endingTextF(sf::Font& font, sf::RenderWindow& window) {
    constexpr int textSize{ 100 };
    constexpr int distanceFromTop{ 100 };

    sf::Text endingText("You won!!!", font, textSize);
    endingText.setOrigin(endingText.getLocalBounds().width/2, endingText.getLocalBounds().height / 2);
    endingText.setPosition(window.getSize().x / 2, distanceFromTop);
    endingText.setFillColor(sf::Color::White);
    return endingText;
}

bool checkCollisionSides(sf::RenderWindow& window, sf::CircleShape& shapeBall, sf::Text& endingText, sf::Text& aiText, sf::Text& playerText, int& tempAiPoints, int& tempPlayerPoints) {
   
    constexpr int distanceFromTop{ 100 };
    const int middleDistance{ static_cast<int>(window.getSize().y / 2)};
    const int topDistance{ static_cast<int>(window.getSize().y - distanceFromTop) };
    
    if (shapeBall.getPosition().x > window.getSize().x - shapeBall.getRadius()) {
        shapeBall.setPosition(window.getSize().x / 2, window.getSize().y / 2);
        tempAiPoints++;
        aiText.setString(std::to_string(tempAiPoints));
        if (tempAiPoints >= 5) {     //Ai won!
            window.clear();
            endingText.setString("You lost idiot!");
            endingText.setOrigin(endingText.getLocalBounds().width / 2, endingText.getLocalBounds().height / 2);
            endingText.setPosition(window.getSize().x / 2, distanceFromTop);
            window.draw(endingText);
            window.display();
            return true;
        }
    }
    else if (shapeBall.getPosition().x < 0 + shapeBall.getRadius()) {
        shapeBall.setPosition(window.getSize().x / 2, window.getSize().y / 2);
        tempPlayerPoints++;
        playerText.setString(std::to_string(tempPlayerPoints));
        if (tempPlayerPoints >= 5) {    //Player won!
            window.clear();
            window.draw(endingText);
            window.display();
            return true;
        }
    }
    return false;
}

void collisionBarCheck(sf::CircleShape& shapeBall, sf::RectangleShape& playerShape, sf::RectangleShape& aiShape, float& xStep) {
    if (shapeBall.getPosition().x > playerShape.getPosition().x - playerShape.getSize().x / 2 - shapeBall.getRadius() &&
        shapeBall.getPosition().y > playerShape.getPosition().y - playerShape.getSize().y / 2 &&
        shapeBall.getPosition().y < playerShape.getPosition().y + playerShape.getSize().y / 2 &&
        shapeBall.getPosition().x < playerShape.getPosition().x - 15) {
        xStep *= -1;
    }
    else if (shapeBall.getPosition().x < aiShape.getPosition().x + aiShape.getSize().x / 2 + shapeBall.getRadius() &&
        shapeBall.getPosition().y > aiShape.getPosition().y - aiShape.getSize().y / 2 &&
        shapeBall.getPosition().y < aiShape.getPosition().y + aiShape.getSize().y / 2 &&
        shapeBall.getPosition().x > aiShape.getPosition().x) {
        xStep *= -1;
    }
}

sf::RectangleShape createButton(int heightY, sf::RenderWindow& window) {
    sf::RectangleShape button{ sf::Vector2f(window.getSize().x / 2, 100) };
    button.setOrigin(button.getGlobalBounds().width / 2, button.getGlobalBounds().height / 2);
    button.setPosition(window.getSize().x / 2, heightY);
    button.setFillColor(sf::Color::White);
    return button;
}
sf::Text createTextButton(std::string name, int heightY, sf::RenderWindow& window, sf::Font& font) {
    constexpr int textAdjustment{ 10 };
    sf::Text text{ name, font, 50 };
    text.setFillColor(sf::Color::Black);
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    text.setPosition(window.getSize().x / 2, heightY - textAdjustment);

    return text;
}

int main()
{
    //var declaration
    constexpr int numberSideX{ 50 };
    float xStep{ 4.98f };
    float yStep{ 4.98f };
    int tempPlayerPoints{ 0 };
    int tempAiPoints{ 0 };
    

    //Initialization
    sf::RenderWindow window;
    windowInitialization(window);

    sf::RectangleShape playerShape{ playerRectangleInitialization(window, numberSideX) };
    sf::RectangleShape aiShape{ aiRectangleInitialization(playerShape, window, numberSideX) };
    sf::RectangleShape middleLine{ middleLineInitialization(window) };
    sf::CircleShape shapeBall{ballInitialization(window)};
    
    sf::Font font{defaultFont()};
    sf::Text playerText{playerTextF(font, window)};
    sf::Text aiText{aiTextF(font, window)};
    sf::Text endingText{endingTextF(font, window)};
    
    while (window.isOpen())
    {
        //get the mouse positionX + move player + move ball
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        int newPosition{ std::clamp(localPosition.y, static_cast<int>(playerShape.getSize().y / 2), static_cast<int>(window.getSize().y - playerShape.getSize().y / 2))};
        playerShape.setPosition(window.getSize().x - numberSideX, static_cast<float>(newPosition));
        shapeBall.move(sf::Vector2f(xStep, yStep));

        //moving Ai
        if (aiShape.getPosition().y > shapeBall.getPosition().y) {
            aiShape.move(0, -4.5);
        }
        else if (aiShape.getPosition().y < shapeBall.getPosition().y) {
            aiShape.move(0, 4.5);
        }

        //collision check    side + top
        bool check{ checkCollisionSides(window, shapeBall, endingText, aiText, playerText, tempAiPoints, tempPlayerPoints) };
        
        if (shapeBall.getPosition().y > window.getSize().y - shapeBall.getRadius()) {
            yStep *= -1;
        }
        else if (shapeBall.getPosition().y < shapeBall.getRadius()) {
            yStep *= -1;
        }
        if (check) {
            bool bCheck{ true };

            //setting 2 buttons and 3 texts
            constexpr int distanceFromTop{ 100 };
            const int middleDistance{ static_cast<int>(window.getSize().y / 2) };
            const int topDistance{ static_cast<int>(window.getSize().y - distanceFromTop) };
            sf::RectangleShape restartButton{ createButton(middleDistance, window)};
            sf::RectangleShape quitButton{ createButton(topDistance, window)};
            sf::Text quitText{ createTextButton("Quit", topDistance, window, font) };
            sf::Text restartText{ createTextButton("Restart", middleDistance, window, font) };

            window.clear();
            window.draw(endingText);
            window.draw(restartButton);
            window.draw(quitButton);
            window.draw(quitText);
            window.draw(restartText);
            window.display();

            while (bCheck) {
                bool bInnerCheck{ false };
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::MouseButtonPressed) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                        if (mousePosition.x > restartButton.getPosition().x - restartButton.getSize().x / 2 && mousePosition.x < restartButton.getPosition().x + restartButton.getSize().x / 2 &&
                            mousePosition.y > restartButton.getPosition().y - restartButton.getSize().y / 2 && mousePosition.y < restartButton.getPosition().y + restartButton.getSize().y / 2) {
                            tempPlayerPoints = 0;
                            tempAiPoints = 0;
                            playerText.setString("0");
                            aiText.setString("0");
                            bInnerCheck = true;
                            break;
                        }
                        else if (mousePosition.x > quitButton.getPosition().x - quitButton.getSize().x / 2 && mousePosition.x < quitButton.getPosition().x + quitButton.getSize().x / 2 &&
                            mousePosition.y > quitButton.getPosition().y - quitButton.getSize().y / 2 && mousePosition.y < quitButton.getPosition().y + quitButton.getSize().y / 2) {
                            return 0;
                        }
                    }
                }
                if (bInnerCheck) {
                    break;
                }
                
            }
            continue;
        }

        collisionBarCheck(shapeBall, playerShape, aiShape, xStep);
       
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(middleLine);
        window.draw(playerShape);
        window.draw(aiShape);
        window.draw(shapeBall);
        window.draw(playerText);
        window.draw(aiText);
        window.display();
    }
  
    return 0;
}
