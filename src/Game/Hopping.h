#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

class Level;

class Hopping
{
  public:
    enum class JobKind
    {
        Walk,
        Dig
    };

    enum class State
    {
        Living,
        Dead,
        ReachedGoal
    };

  public:
    Hopping();

    sf::IntRect getBounds() const;

    void onUpdate(Level& level);

    void onRender(sf::RenderWindow* window, sf::RectangleShape& sprite);

    void setJob(JobKind job);

    State getState() const
    {
        return m_state;
    }

  private:
    void walk(Level& level, sf::Vector2f& movement);
    void dig(Level& level);

    JobKind m_job = JobKind::Walk;

    sf::Vector2f m_position;
    sf::Vector2f m_size;

    bool m_isOnGround = false;
    int m_direction = 1;

    bool m_hasReachedEnd;

    State m_state = State::Living;
    int m_fallTime = 0;
};