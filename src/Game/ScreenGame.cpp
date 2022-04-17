#include "ScreenGame.h"
#include "ScreenMainMenu.h"
#include <cmath>
#include <imgui_sfml/imgui.h>
#include <iostream>

ScreenGame::ScreenGame(ScreenManager* stack)
    : Screen(stack)
{
    m_levelImage.loadFromFile("data/levels/level1/bg.png");
    m_levelTexture.loadFromImage(m_levelImage);
    m_levelSprite.setTexture(&m_levelTexture);
    m_levelSprite.setSize({m_levelImage.getSize().x, m_levelImage.getSize().y});

    m_hoppingSprite.setSize({16, 16});
    m_hoppingSprite.setFillColor(sf::Color::Yellow);

    m_hoppingSprite.setPosition(310, 200);
}

void ScreenGame::onEvent(const sf::Event& e)
{
    if (e.type == sf::Event::KeyReleased) {
        if (e.key.code == sf::Keyboard::D) {
            m_hoppingSprite.move(1, 0);
        }
        if (e.key.code == sf::Keyboard::A) {
            m_hoppingSprite.move(-1, 0);
        }
    }
}

void ScreenGame::onGUI()
{
    ImGui::Begin("Menu");
    if (ImGui::Button("Exit Game")) {
        m_pScreens->popScreen();
    }
    ImGui::End();
}

void ScreenGame::onFixedUpdate(const sf::Time& dt)
{

    // Test for solid pixels under the m_hoppingSprite
    sf::Vector2f pos = m_hoppingSprite.getPosition();
    sf::Vector2f size = m_hoppingSprite.getSize();

    sf::Vector2f movement{0, 0};

    m_isgrounded = false;
    for (int x = pos.x; x < pos.x + size.x; x++) {
        if (m_levelImage.getPixel(x, pos.y + size.y).a != 0) {
            m_isgrounded = true;
            break;
        }
    }

    if (m_isgrounded) {
        for (int y = pos.y + size.y - 1; y >= pos.y; y -= size.y / 4) {
            std::cout << y << '\n';
            if (m_levelImage.getPixel(pos.x + size.x, y).a != 0) {
                int wallsize = 0;
                while (m_levelImage.getPixel(pos.x + size.x, y - wallsize).a != 0) {
                    wallsize++;
                }
                movement.y -= wallsize;
            }
        }
        movement.x += 1;
    }
    else {
        movement.y += 1;
    }

    m_hoppingSprite.move(movement);
}

void ScreenGame::onRender(sf::RenderWindow* window)
{

    window->draw(m_levelSprite);

    window->draw(m_hoppingSprite);

    // reload the image into the texture
    m_levelTexture.loadFromImage(m_levelImage);
}
