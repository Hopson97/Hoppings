#include "ScreenGame.h"
#include "ScreenMainMenu.h"
#include <cmath>
#include <imgui_sfml/imgui.h>
#include <iostream>

const int SPEED = 1;

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
    if (ImGui::Button("Exit Game"))
    {
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

    m_isOnGround = false;
    for (int x = pos.x; x < pos.x + size.x; x++)
    {
        if (isGround({x, pos.y + size.y}))
        {
            m_isOnGround = true;
            break;
        }
    }

    if (m_isOnGround)
    {
        const int GROUND_Y = pos.y + size.y - 1;
        for (int y = GROUND_Y; y >= pos.y; y -= size.y / 4)
        {
            if (y == GROUND_Y && isGround({pos.x + size.x, y}))
            {
                int wallsize = 0;
                while (isGround({pos.x + size.x, y - wallsize}))
                {
                    wallsize++;
                }
                movement.y -= wallsize;
            }
        }
        movement.x += SPEED;
    }
    else
    {
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

bool ScreenGame::outOfImageBounds(const sf::Vector2i& position) const
{
    const auto SIZE = m_levelImage.getSize();
    return position.x < 0 || position.y < 0 || position.x >= SIZE.x ||
           position.y >= SIZE.y;
}

bool ScreenGame::isGround(const sf::Vector2i& position) const
{
    if (outOfImageBounds(position))
    {
        return false;
    }
    // Check if the pixel is solid aka not transparent
    return m_levelImage.getPixel(position.x, position.y).a != 0;
}
bool ScreenGame::isAir(const sf::Vector2i& position) const
{
    if (outOfImageBounds(position))
    {
        return false;
    }

    // Check if the pixel is solid aka transparent
    return m_levelImage.getPixel(position.x, position.y).a == 0;
}