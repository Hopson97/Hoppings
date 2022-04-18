#include "Hopping.h"
#include "Level.h"
#include <iostream>

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

    if (m_state == State::Living)
    {

        // Test for solid pixels under the m_hoppingSprite
        m_isOnGround = false;
        for (int x = m_position.x; x < m_position.x + m_size.x; x++)
        {
            if (level.isGround({x, m_position.y + m_size.y}))
            {
                if (m_fallTime > m_size.y * 5.0f)
                {
                    m_state = State::Dead;
                    break;
                }
                else
                {
                    m_fallTime = 0;
                    m_isOnGround = true;
                    break;
                }
            }
        }
    }

    // Another if as they may have died in the previous if
    if (m_state == State::Living)
    {

        if (!m_isOnGround)
        {
            movement.y += 1;
            m_fallTime++;
        }

        switch (m_job)
        {
            case JobKind::Walk:
                walk(level, movement);
                break;

            case JobKind::Dig:
                dig(level);
                break;
        }

        m_position += movement;
        // Set state to dead if the hopping is out of the level
        // if (m_position.x < 0 || m_position.x + m_size.x > level.getSize().x ||
        //      m_position.y < 0 || m_position.y + m_size.y > level.getSize().y)
        //  {
        //      m_state = State::Dead;
        //  }

        // Set state to reached goal if the hopping reached the end of the level
        if (getBounds().intersects(level.getWinArea()))
        {
            m_state = State::ReachedGoal;
        }
    }
}

void Hopping::onRender(sf::RenderWindow* window, sf::RectangleShape& sprite)
{
    if (m_state != State::Dead)
    {
        sprite.setSize(m_size);
        sprite.setPosition(m_position);
        window->draw(sprite);
    }
}

void Hopping::setJob(JobKind job)
{
    m_job = job;
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
                while (wallSize < m_size.y &&
                       level.isGround({(int)m_position.x + OFFSET, y - wallSize}))
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

void Hopping::dig(Level& level)
{
    if (m_isOnGround)
    {
        for (int x = m_position.x - 5; x < m_position.x + m_size.x + 5; x++)
        {
            if (level.isGround({x, m_position.y + m_size.y}))
            {
                level.dig({x, m_position.y + m_size.y});
            }
        }
    }
    else if (m_fallTime > 2)
    {
        m_job = JobKind::Walk;
    }
}