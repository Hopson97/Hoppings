#include "Hopping.h"
#include "Level.h"

Hopping::Hopping()
{
    m_size = {16, 16};
    m_position = {310, 200};
}

void Hopping::onUpdate(const Level& level)
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

    if (m_isOnGround)
    {
        const int GROUND_Y = m_position.y + m_size.y - 1;
        for (int y = GROUND_Y; y >= m_position.y; y -= m_size.y / 4)
        {
            const int OFFSET = m_size.y * m_direction;
            if (y == GROUND_Y && level.isGround({m_position.x + OFFSET, y}))
            {
                int wallm_size = 0;
                while (wallm_size < m_size.y < 2 &&
                       level.isGround({m_position.x + OFFSET, y - wallm_size}))
                {
                    wallm_size++;
                }

                // Either climb the wall instantly, or reverse direction
                if (wallm_size > m_size.y / 2)
                {
                    m_direction = !m_direction;
                }
                else
                {
                    movement.y -= wallm_size;
                }
            }
        }
        movement.x += m_direction == 1 ? 1 : -1;
    }
    else
    {
        movement.y += 1;
    }

    m_position += movement;
}

void Hopping::onRender(sf::RenderWindow* window, sf::RectangleShape& sprite)
{
    sprite.setSize(m_size);
    sprite.setPosition(m_position);
    window->draw(sprite);
}