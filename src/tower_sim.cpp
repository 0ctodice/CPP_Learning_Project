#include "tower_sim.hpp"

#include "aircraft.hpp"
#include "config.hpp"
#include "img/image.hpp"

#include <cassert>

using namespace std::string_literals;

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    context          = std::make_unique<ContextInitializer>(argc, argv);
    aircraft_manager = std::make_unique<AircraftManager>();
    aircraft_factory = std::make_unique<AircraftFactory>();
    GL::move_queue.emplace(aircraft_manager.get());

    create_keystrokes();
}

void TowerSimulation::create_random_aircraft() const
{
    assert(airport);
    aircraft_manager->add_aircraft(aircraft_factory->create_aircraft(airport->get_tower()));
}

void TowerSimulation::print_existing_aircraft(int i)
{
    assert(i >= 0 && i < 8);
    auto airline = aircraft_factory->airlines[i];
    std::cout << "There is "
              << std::count_if(aircraft_manager->aircrafts.begin(), aircraft_manager->aircrafts.end(),
                               [this, i, airline](const std::unique_ptr<Aircraft>& aircraft)
                               { return aircraft->get_flight_num().substr(0, 2).compare(airline) == 0; })
              << " " << airline << " airlines" << std::endl;
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { create_random_aircraft(); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('*', []() { GL::ticks_per_sec = std::min(GL::ticks_per_sec + 1u, 200u); });
    GL::keystrokes.emplace('!', []() { GL::ticks_per_sec = std::max(GL::ticks_per_sec - 1u, 1u); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('p', []() { GL::is_paused = !GL::is_paused; });
    GL::keystrokes.emplace(
        'm', [this]()
        { std::cout << "total landing crashes : " << aircraft_manager->landing_crash_counter << std::endl; });
    for (int i = 0; i < 8; i++)
    {
        GL::keystrokes.emplace(i + '0', [this, i]() { print_existing_aircraft(i); });
    }
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [key, value] : GL::keystrokes)
    {
        std::cout << key << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    assert(!airport);
    airport = std::make_unique<Airport>(one_lane_airport, Point3D { 0, 0, 0 },
                                        new img::Image { one_lane_airport_sprite_path.get_full_path() },
                                        aircraft_manager.get());

    GL::move_queue.emplace(airport.get());
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();

    GL::loop();
}
