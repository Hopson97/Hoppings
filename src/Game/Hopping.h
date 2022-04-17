#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

class Level;

class Hopping
{
  public:
    Hopping();

    void onUpdate(const Level& level);

    void onRender(sf::RenderWindow* window, sf::RectangleShape& sprite);

  private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    bool m_isOnGround = false;
    int m_direction = 1;
};