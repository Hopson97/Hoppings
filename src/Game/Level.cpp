#include "Level.h"

Level::Level()
{
    m_levelImage.loadFromFile("data/levels/level1/bg.png");
    m_levelTexture.loadFromImage(m_levelImage);
    m_levelSprite.setTexture(&m_levelTexture);
    m_levelSprite.setSize({m_levelImage.getSize().x, m_levelImage.getSize().y});
}

void Level::onUpdate()
{
}

void Level::onRender(sf::RenderWindow* window)
{

    window->draw(m_levelSprite);

    // reload the image into the texture
    m_levelTexture.loadFromImage(m_levelImage);
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