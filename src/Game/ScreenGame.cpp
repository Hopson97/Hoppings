#include "ScreenGame.h"
#include "ScreenMainMenu.h"
#include <algorithm>
#include <imgui_sfml/imgui.h>
#include <iostream>

ScreenGame::ScreenGame(ScreenManager* stack)
    : Screen(stack)
{
    m_hoppingSprite.setFillColor(sf::Color::Yellow);
    m_level.loadLevel(1);
}

void ScreenGame::onEvent(const sf::Event& e)
{
    if (e.type == sf::Event::MouseButtonReleased)
    {
        if (e.mouseButton.button == sf::Mouse::Left)
        {
            for (auto& hopping : m_hoppings)
            {
                if (hopping.getBounds().contains(e.mouseButton.x, e.mouseButton.y))
                {
                    hopping.setJob(Hopping::JobKind::Dig);
                }
            }
        }
        else if (e.mouseButton.button == sf::Mouse::Right)
        {
            for (auto& hopping : m_hoppings)
            {
                if (hopping.getBounds().contains(e.mouseButton.x, e.mouseButton.y))
                {
                    hopping.setJob(Hopping::JobKind::Walk);
                }
            }
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

    // This will create 1 "Hopping" every 3 seconds at 1x speed
    if ((m_ticks % (Screen::getTickRate() * 3) == 0 || m_ticks == 1) &&
        m_level.getMeta().hoppings > (int)m_hoppings.size())
    {
        m_hoppings.emplace_back();
    }

    for (auto& hopping : m_hoppings)
    {
        hopping.onUpdate(m_level);
    }

    auto completed =
        std::count_if(m_hoppings.cbegin(), m_hoppings.cend(),
                      [](const Hopping& hopping)
                      { return hopping.getState() == Hopping::State::ReachedGoal; });

    if (completed >= m_level.getMeta().goal)
    {
        m_pScreens->changeScreen(std::make_unique<ScreenMainMenu>(m_pScreens));
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
