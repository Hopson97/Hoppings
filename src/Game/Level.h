#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

struct LevelMeta
{
    std::string name;
    std::string description;
    int hoppings = 0;
    int goal = 0;

    sf::Vector2i startPoint;
    sf::Vector2i endPoint;
};

class Level
{
  public:
    Level();

    void loadLevel(int level);

    bool isGround(const sf::Vector2i& position) const;
    bool isAir(const sf::Vector2i& position) const;

    void onUpdate();
    void onRender(sf::RenderWindow* window);

    void dig(const sf::Vector2i& position);

    const LevelMeta& getMeta() const;

  private:
    bool outOfImageBounds(const sf::Vector2i& position) const;

    LevelMeta m_meta;
    sf::Image m_levelImage;
    sf::Texture m_levelTexture;
    sf::RectangleShape m_levelSprite;
};