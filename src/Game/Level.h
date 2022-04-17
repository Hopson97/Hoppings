#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

class Level
{
  public:
    Level();

    bool isGround(const sf::Vector2i& position) const;
    bool isAir(const sf::Vector2i& position) const;

    void onUpdate();
    void onRender(sf::RenderWindow* window);

  private:
    bool outOfImageBounds(const sf::Vector2i& position) const;

    sf::Image m_levelImage;
    sf::Texture m_levelTexture;
    sf::RectangleShape m_levelSprite;
};