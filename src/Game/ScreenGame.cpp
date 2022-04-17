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

    if (ImGui::Button("1x Speed"))
    {
        m_tickRate = 1;
    }
    if (ImGui::Button("2x Speed"))
    {
        m_tickRate = 2;
    }
    if (ImGui::Button("3x Speed"))
    {
        m_tickRate = 3;
    }
    if (ImGui::Button("4x Speed"))
    {
        m_tickRate = 4;
    }
    if (ImGui::Button("5x Speed"))
    {
        m_tickRate = 5;
    }
    if (ImGui::Button("6x Speed"))
    {
        m_tickRate = 6;
    }
    if (ImGui::Button("7x Speed"))
    {
        m_tickRate = 7;
    }
    if (ImGui::Button("8x Speed"))
    {
        m_tickRate = 8;
    }
    if (ImGui::Button("9x Speed"))
    {
        m_tickRate = 9;
    }
    if (ImGui::Button("10x Speed"))
    {
        m_tickRate = 10;
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
            const int OFFSET = size.y * m_direction;
            if (y == GROUND_Y && isGround({pos.x + OFFSET, y}))
            {
                int wallsize = 0;
                while (wallsize < size.y < 2 && isGround({pos.x + OFFSET, y - wallsize}))
                {
                    wallsize++;
                }

                // Either climb the wall instantly, or reverse direction
                if (wallsize > size.y / 2)
                {
                    m_direction = !m_direction;
                }
                else
                {
                    movement.y -= wallsize;
                }
            }
        }
        movement.x += m_direction == 1 ? SPEED : -SPEED;
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

int ScreenGame::getTickRate() const
{
    return Screen::getTickRate() * m_tickRate;
}

bool ScreenGame::outOfImageBounds(const sf::Vector2i& position) const
{
    const auto SIZE = m_levelImage.getSize();
    return position.x < 0 || position.y < 0 || position.x >= (int)SIZE.x ||
           position.y >= (int)SIZE.y;
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