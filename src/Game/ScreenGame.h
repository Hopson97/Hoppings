#pragma once

#include "../Screen.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Level.h"

class ScreenGame final : public Screen
{
    enum class Menu
    {
        MainMenu,
        SettingsMenu,
    };

  public:
    ScreenGame(ScreenManager* screens);

    void onEvent(const sf::Event& e) override;
    void onGUI() override;
    void onFixedUpdate(const sf::Time& dt) override;
    void onRender(sf::RenderWindow* window) override;

    int getTickRate() const override;

  private:
    bool outOfImageBounds(const sf::Vector2i& position) const;
    bool isGround(const sf::Vector2i& position) const;
    bool isAir(const sf::Vector2i& position) const;

    Level m_level;

    Menu m_activeMenu = Menu::MainMenu;
    sf::Clock m_clock;

    sf::RectangleShape m_hoppingSprite;
    bool m_isOnGround = false;
    int m_direction = 1;
    int m_tickRate = 1;
};
