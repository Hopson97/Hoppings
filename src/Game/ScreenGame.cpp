#include "ScreenGame.h"
#include "ScreenMainMenu.h"
#include <cmath>
#include <imgui_sfml/imgui.h>
#include <iostream>

ScreenGame::ScreenGame(ScreenManager* stack)
    : Screen(stack)
{
    m_hoppingSprite.setFillColor(sf::Color::Yellow);
}

void ScreenGame::onEvent(const sf::Event& e)
{
    if (e.type == sf::Event::KeyReleased)
    {
        if (e.key.code == sf::Keyboard::D)
        {
            m_hoppingSprite.move(1, 0);
        }
        if (e.key.code == sf::Keyboard::A)
        {
            m_hoppingSprite.move(-1, 0);
        }
    }
}

void ScreenGame::onGUI()
{
    ImGui::Begin("Menu");

    char buf[256];
    sprintf(buf, "FPS: %d", m_tickRate);

    for (int i = 1; i <= 10; i++)
    {
        sprintf(buf, "%dx Speed", i);
        if (ImGui::Button(buf))
        {
            m_tickRate = i;
        }
    }

    ImGui::Text("Current Speed: %dx", m_tickRate);

    if (ImGui::Button("Exit Game"))
    {
        m_pScreens->popScreen();
    }
    ImGui::End();
}

void ScreenGame::onFixedUpdate([[maybe_unused]] const sf::Time& dt)
{
    m_ticks++;

    // This will create 1 "Hopping" per second at 1x speed
    if (m_ticks % Screen::getTickRate() == 0)
    {
        m_hoppings.emplace_back();
    }

    for (auto& hopping : m_hoppings)
    {
        hopping.onUpdate(m_level);
    }
}

void ScreenGame::onRender(sf::RenderWindow* window)
{
    // Draw level
    m_level.onRender(window);

    for (auto& hopping : m_hoppings)
    {
        hopping.onRender(window, m_hoppingSprite);
    }
}

int ScreenGame::getTickRate() const
{
    return Screen::getTickRate() * m_tickRate;
}
