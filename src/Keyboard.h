#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>

class Keyboard
{
  public:
    Keyboard()
    {
        std::fill(m_keys.begin(), m_keys.end(), false);
    }

    bool isKeyDown(sf::Keyboard::Key key) const
    {
        return m_keys[(size_t)key];
    }

    void update(sf::Event event)
    {
        if (auto* key = event.getIf<sf::Event::KeyPressed>())
        {
            if ((size_t)key->code < m_keys.size())
            {
                m_keys[(size_t)key->code] = true;
            }
        }
        else if (auto* key = event.getIf<sf::Event::KeyReleased>())
        {
            if ((size_t)key->code < m_keys.size())
            {
                m_keys[(size_t)key->code] = false;
            }
        }
    }

  private:
    std::array<bool, sf::Keyboard::KeyCount> m_keys{false};
};