#include "Level.h"

#include <fstream>

#include <nlohmann/json.hpp>

Level::Level()
{
    m_areaSprite.setSize({32, 32});
}

void Level::loadLevel(int level)
{
    m_levelImage.loadFromFile("data/levels/lvl" + std::to_string(level) + "/bg.png");
    m_levelTexture.loadFromImage(m_levelImage);
    m_levelSprite.setSize({m_levelImage.getSize().x, m_levelImage.getSize().y});
    m_levelSprite.setTexture(&m_levelTexture);

    std::ifstream levelStream("data/levels/lvl" + std::to_string(level) + "/meta.json");
    nlohmann::json levelJson;
    levelStream >> levelJson;

    m_meta.name = levelJson["name"];
    m_meta.description = levelJson["description"];
    m_meta.hoppings = levelJson["hoppings"];
    m_meta.goal = levelJson["goal"];

    m_meta.startPoint = {levelJson["startPoint"]["x"], levelJson["startPoint"]["y"]};
    m_meta.endPoint = {levelJson["endPoint"]["x"], levelJson["endPoint"]["y"]};

    m_winArea = {
        getMeta().endPoint.x,
        getMeta().endPoint.y,
        32,
        32,
    };
}

void Level::onUpdate()
{
}

void Level::onRender(sf::RenderWindow* window)
{
    // Draw a big square around the start point
    m_areaSprite.setPosition(m_meta.startPoint.x - 8, m_meta.startPoint.y - 8);
    m_areaSprite.setFillColor(sf::Color::Green);
    window->draw(m_areaSprite);

    // Draw a big square around the end point
    m_areaSprite.setPosition(m_meta.endPoint.x - 8, m_meta.endPoint.y - 8);
    m_areaSprite.setFillColor(sf::Color::Red);
    window->draw(m_areaSprite);

    // reload the image into the texture
    m_levelTexture.loadFromImage(m_levelImage);
    window->draw(m_levelSprite);
}

bool Level::outOfImageBounds(const sf::Vector2i& position) const
{
    const auto SIZE = m_levelImage.getSize();
    return position.x < 0 || position.y < 0 || position.x >= (int)SIZE.x ||
           position.y >= (int)SIZE.y;
}

bool Level::isGround(const sf::Vector2i& position) const
{
    if (outOfImageBounds(position))
    {
        return false;
    }
    // Check if the pixel is solid aka not transparent
    return m_levelImage.getPixel(position.x, position.y).a != 0;
}

bool Level::isAir(const sf::Vector2i& position) const
{
    if (outOfImageBounds(position))
    {
        return false;
    }

    // Check if the pixel is solid aka transparent
    return m_levelImage.getPixel(position.x, position.y).a == 0;
}

void Level::dig(const sf::Vector2i& position)
{
    if (outOfImageBounds(position))
    {
        return;
    }

    m_levelImage.setPixel(position.x, position.y, sf::Color::Transparent);
}