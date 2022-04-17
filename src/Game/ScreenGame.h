#pragma once

#include "../Screen.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

class ScreenGame final : public Screen {
    enum class Menu {
        MainMenu,
        SettingsMenu,
    };

  public:
    ScreenGame(ScreenManager* screens);

    void onEvent(const sf::Event& e) override;
    void onGUI() override;
    void onFixedUpdate(const sf::Time& dt) override;
    void onRender(sf::RenderWindow* window) override;

  private:
    Menu m_activeMenu = Menu::MainMenu;
    sf::Clock m_clock;

    sf::Image m_levelImage;
    sf::Texture m_levelTexture;
    sf::RectangleShape m_levelSprite;

    sf::RectangleShape m_hoppingSprite;
    bool m_isgrounded = false;
};
