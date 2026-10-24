#include "Game/ScreenGame.h"
#include "Game/ScreenMainMenu.h"
#include "Keyboard.h"
#include "Profiler.h"
#include "Screen.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui.h>
#include <imgui_sfml/imgui-SFML.h>

int main()
{
    // Set up window and gui
    sf::RenderWindow window(sf::VideoMode{{1600, 900}}, "Hoppings");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    ImGui::SFML::Init(window);

    Profiler profiler;

    // Update ImGUI UI scaling for 4K monitors. For some reason looks bad on linux hence
    // the ifdef
#ifndef __linux__
    if (sf::VideoMode::getDesktopMode().size.x > 3600)
    {
        ImGui::GetIO().FontGlobalScale = ((window.getSize().x) / 1024.f);
    }
#endif
    // Set up screen system
    ScreenManager screens;
    screens.pushScreen(std::make_unique<ScreenGame>(&screens));
    screens.update();

    // Time step, 30 ticks per second
    sf::Clock timer;
    sf::Time lastTime = sf::Time::Zero;
    sf::Time lag = sf::Time::Zero;

    // Misc/ Util
    Keyboard keyboard;
    sf::Clock updateClock;

    while (window.isOpen() && !screens.isEmpty())
    {
        Screen* screen = &screens.peekScreen();
        profiler.reset();

        {

            TimeSlot& profilerSlot = profiler.newTimeslot("Events");

            while (window.pollEvent())
            {

                while (auto event = window.pollEvent())
                {
                    ImGui::SFML::ProcessEvent(window, *event);
                    keyboard.update(*event);
                    screen->onEvent(*event);

                    if (event->is<sf::Event::Closed>())
                    {
                        window.close();
                    }
                    if (auto key = event->getIf<sf::Event::KeyReleased>())
                    {
                        if (key->code == sf::Keyboard::Key::Escape)
                        {
                            window.close();
                        }
                    }
                }
                profilerSlot.stop();
            }

            // Get times
            sf::Time dt = updateClock.restart();
            sf::Time time = timer.getElapsedTime();
            sf::Time elapsed = time - lastTime;
            lastTime = time;
            lag += elapsed;

            // User input (real time)
            {
                TimeSlot& profilerSlot = profiler.newTimeslot("Input");
                screen->onInput(keyboard, window);
                profilerSlot.stop();
            }

            // Game update
            {
                TimeSlot& profilerSlot = profiler.newTimeslot("Update");
                screen->onUpdate(dt);
                ImGui::SFML::Update(window, dt);
                profilerSlot.stop();
            }

            // Fixed update
            {
                TimeSlot& profilerSlot = profiler.newTimeslot("Fixed Update");
                // Fixed time update
                sf::Time timePerUpdate = sf::seconds(1.0f / screen->getTickRate());
                while (lag >= timePerUpdate)
                {
                    lag -= timePerUpdate;
                    screen->onFixedUpdate(elapsed);
                }
                profilerSlot.stop();
            }

            // Rendering
            window.clear({16, 4, 2});
            {
                TimeSlot& profilerSlot = profiler.newTimeslot("Render");
                screen->onRender(&window);
                profilerSlot.stop();
            }

            // GUI/ ImGUI stuff
            {
                TimeSlot& profilerSlot = profiler.newTimeslot("GUI");
                screen->onGUI();
                profilerSlot.stop();
            }

            profiler.onGUI();
            ImGui::SFML::Render(window);
            window.display();
            screens.update();
        }
    }
}