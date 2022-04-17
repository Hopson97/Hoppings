#include "Hopping.h"
#include "Level.h"

Hopping::Hopping()
{
    m_size = {16, 16};
    m_position = {310, 200};
}

sf::IntRect Hopping::getBounds() const
{
    return sf::IntRect(m_position.x, m_position.y, m_size.x, m_size.y);
}

void Hopping::onUpdate(Level& level)
{

    sf::Vector2f movement{0, 0};
    // Test for solid pixels under the m_hoppingSprite

    m_isOnGround = false;
    for (int x = m_position.x; x < m_position.x + m_size.x; x++)
    {
        if (level.isGround({x, m_position.y + m_size.y}))
        {
            m_isOnGround = true;
            break;
        }
    }

    if (!m_isOnGround)
    {
        movement.y += 1;
    }

    switch (m_workType)
    {
        case WorkType::Walk:
            walk(level, movement);
            break;

        case WorkType::Dig:
            dig(level, movement);
            break;
    }

    m_position += movement;
}

void Hopping::onRender(sf::RenderWindow* window, sf::RectangleShape& sprite)
{
    sprite.setSize(m_size);
    sprite.setPosition(m_position);
    window->draw(sprite);
}

void Hopping::setJob(WorkType job)
{
    m_workType = job;
}

void Hopping::walk(Level& level, sf::Vector2f& movement)
{
    if (m_isOnGround)
    {
        const int GROUND_Y = m_position.y + m_size.y - 1;
        for (int y = GROUND_Y; y >= m_position.y; y -= m_size.y / 4)
        {
            const int OFFSET = m_size.y * m_direction;
            if (y == GROUND_Y && level.isGround({m_position.x + OFFSET, y}))
            {
                int wallSize = 0;
                while (wallSize < m_size.y < 2 &&
                       level.isGround({m_position.x + OFFSET, y - wallSize}))
                {
                    wallSize++;
                }

                // Either climb the wall instantly, or reverse direction
                if (wallSize > m_size.y / 2)
                {
                    m_direction = !m_direction;
                }
                else
                {
                    movement.y -= wallSize;
                }
            }
        }
        movement.x += m_direction == 1 ? 1 : -1;
    }
}

void Hopping::dig(Level& level, sf::Vector2f& movement)
{
    if (m_isOnGround)
    {
        for (int x = m_position.x; x < m_position.x + m_size.x; x++)
        {
            if (level.isGround({x, m_position.y + m_size.y}))
            {
                level.dig({x, m_position.y + m_size.y});
            }
        }
    }
}