#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

class Level;

class Hopping
{
  public:
    enum class WorkType
    {
        Walk,
        Dig
    };

  public:
    Hopping();

    sf::IntRect getBounds() const;

    void onUpdate(Level& level);

    void onRender(sf::RenderWindow* window, sf::RectangleShape& sprite);

    void setJob(WorkType job);

  private:
    void walk(Level& level, sf::Vector2f& movement);
    void dig(Level& level, sf::Vector2f& movement);

    WorkType m_workType = WorkType::Walk;

    sf::Vector2f m_position;
    sf::Vector2f m_size;

    bool m_isOnGround = false;
    int m_direction = 1;
};