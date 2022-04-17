#pragma once

#include <vector>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../Screen.h"
#include "Hopping.h"
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
    std::vector<Hopping> m_hoppings;
    Level m_level;

    Menu m_activeMenu = Menu::MainMenu;
    sf::Clock m_clock;

    sf::RectangleShape m_hoppingSprite;
    int m_tickRate = 1;

    int m_ticks = 0;
};
